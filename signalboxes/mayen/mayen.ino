#include <Bounce2.h>

// Red LEDs named after the signal/switch lever/etc they're for.
// These are located at the analog ports.
int PIN_LED_ROUTE = A0;      // Fahrstrassenhebel
int PIN_LED_SWITCH_5 = A1;   // Left switch  (-> Kelberg)
int PIN_LED_SWITCH_1 = A2;   // Right switch (-> Mayen)
int PIN_LED_LOCK = A3;       // Zugfuhrerschlussel/Rangierschlussel

// Levers and lock
int PIN_LOCK = 8;             // Zugfuhrerschlussel/Rangierschlussel
int PIN_LEVER_SWITCH_1 = 10;  // Right switch (-> Mayen)
int PIN_LEVER_SWITCH_5 = 11;  // Left switch  (-> Kelberg)
// Route levers
int PIN_LEVER_ROUTE_1 = 12;   // Fahrstrassenhebel Gleis 1 (Grundstellung)
int PIN_LEVER_ROUTE_2 = 13;   // Fahrstrassenhebel Gleis 2

// Output pins (pull to 0 means go out of the ground position)
int PIN_OUTPUT_SWITCH_1 = 6;  // Right switch (-> Mayen)
int PIN_OUTPUT_SWITCH_5 = 7;  // Left switch  (-> Kelberg)


// Array index numbers. These are columns in the locking table.
int SWITCH_5 = 0;  // REST means PLUS/rechtuit.
int SWITCH_1 = 1;  // REST means PLUS/rechtuit.
int LOCK = 2;  // Note: ACTUATED means 'Zugfuhrerschlussel can be withdrawn'.
int ROUTE_1 = 3;
int ROUTE_2 = 4;

// The two possible positions. Plus is the rest state, minus the actuated state.
bool REST = false;
bool ACTUATED = true;

// The states. All arrays with length 5.
const int ARRAY_SIZE = 5;
boolean current_position [ARRAY_SIZE] = { REST, REST, ACTUATED, ACTUATED, REST };
boolean blocked [ARRAY_SIZE] = { false, false, false, false, false };
boolean requirements_fulfilled [ARRAY_SIZE] = { true, true, true, true, false };
boolean position_is_ok [ARRAY_SIZE] = { true, true, true, true, true };

// Actual levers/buttons/locks.
Bounce levers [ARRAY_SIZE];


boolean movement_allowed (int number) {
  return (blocked[number] == false && requirements_fulfilled[number] == true);
}


void update_requirements() {
  // Requirements flow "upwards". The correct switch positions can enable
  // routes. Enabled route 1 allows the lock to be activated.

  // Requirements fulfilled by the switches.
  requirements_fulfilled[ROUTE_1] = false;
  requirements_fulfilled[ROUTE_2] = false;
  requirements_fulfilled[LOCK] = false;
  if (current_position[SWITCH_5] == REST &&
      current_position[SWITCH_1] == REST) {
    Serial.println("Weichen beide REST -> Fahrstrasse 1 ist erlaubt");
    requirements_fulfilled[ROUTE_1] = true;
  }
  if (current_position[SWITCH_5] == ACTUATED &&
      current_position[SWITCH_1] == ACTUATED) {
    Serial.println("Weichen beide ACTUATED -> Fahrstrasse 2 ist erlaubt");
    requirements_fulfilled[ROUTE_2] = true;
  }
  if (current_position[ROUTE_1] == ACTUATED) {
    Serial.println("Fahrstrasse 1 ACTUATED -> Stellwerk kann verschlossen werden");
    requirements_fulfilled[LOCK] = true;
  }

}


void update_blocks() {
  // Blocks flow "downwards". A route lever blocks switches. A route fixation
  // blocks a route lever.

  // Reset all blocks.
  for (int number = 0; number < ARRAY_SIZE; number++) {
    blocked[number] = false;
  }

  // The lock blocks routes.
  // Note: REST means 'Zugfuhrerschlussel can be (or is) withdrawn'.
  if (current_position[LOCK] == ACTUATED) {
    Serial.println("Lock ACTUATED -> Fahrstrasse 1 geblockt");
    blocked[ROUTE_1] = true;
  }

  // Route levers block each other. And they block switches.
  if (current_position[ROUTE_1] == ACTUATED) {
    Serial.println("Fahrstrasse 1 ACTUATED -> beide Weichen und F2 verschlossen");
    blocked[ROUTE_2] = true;
    blocked[SWITCH_5] = true;
    blocked[SWITCH_1] = true;
  }
  if (current_position[ROUTE_2] == ACTUATED) {
    Serial.println("Fahrstrasse 2 ACTUATED -> beide Weichen und F1 verschlossen");
    blocked[ROUTE_1] = true;
    blocked[SWITCH_5] = true;
    blocked[SWITCH_1] = true;
  }

}


void update_outputs() {
  // Active: pull to zero.
  if (current_position[SWITCH_1] == REST) {
    digitalWrite(PIN_OUTPUT_SWITCH_1, HIGH);
  }
  else {
    digitalWrite(PIN_OUTPUT_SWITCH_1, LOW);
  }
  if (current_position[SWITCH_5] == REST) {
    digitalWrite(PIN_OUTPUT_SWITCH_5, HIGH);
  }
  else {
    digitalWrite(PIN_OUTPUT_SWITCH_5, LOW);
  }
}


void change_position(int number,
                     boolean new_position) {
  current_position[number] = new_position;
  position_is_ok[number] = true;

  update_requirements();
  update_blocks();
  update_outputs();
}


void react_to_movement(int number,
                       boolean new_position) {
  Serial.print("Provisional movement of ");
  Serial.print(number);
  Serial.print(" to state ");
  Serial.println(new_position);
  if (movement_allowed(number)) {
    Serial.println("The movement is allowed");
    change_position(number, new_position);
  }
  else {
    if (current_position[number] == new_position) {
      // We're back to the allowed position
      position_is_ok[number] = true;
    }
    else {
      // We're trying to move to a wrong position;
      position_is_ok[number] = false;
    }
  }
}


void update_single_lever_led(int pin, int number) {
  if (position_is_ok[number] == false) {
    if ((millis() % 500) > 250) {
      digitalWrite(pin, HIGH);
    }
    else {
      digitalWrite(pin, LOW);
    }
  }
  else {
    if (movement_allowed(number)) {
      digitalWrite(pin, HIGH);
    }
    else {
      digitalWrite(pin, LOW);
    }
  }
}


void update_dual_lever_led(int pin, int number1, int number2) {
  if (position_is_ok[number1] == false || position_is_ok[number2] == false) {
    if ((millis() % 500) > 250) {
      digitalWrite(pin, HIGH);
    }
    else {
      digitalWrite(pin, LOW);
    }
  }
  else {
    if (movement_allowed(number1) || movement_allowed(number2)) {
      digitalWrite(pin, HIGH);
    }
    else {
      digitalWrite(pin, LOW);
    }
  }

}


void setup() {
  Serial.begin(9600);

  // Attach the pins and hook up react_to_movement() functions.
  // for (int number = 0; number < ARRAY_SIZE; number++) {
  //   levers[number] = Bounce();
  // }

  // Define lever pin modes.
  pinMode(PIN_LEVER_SWITCH_5, INPUT_PULLUP);
  pinMode(PIN_LEVER_SWITCH_1, INPUT_PULLUP);
  pinMode(PIN_LOCK, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_1, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_2, INPUT_PULLUP);

  // Attach the LEDs.
  pinMode(PIN_LED_SWITCH_5, OUTPUT);
  pinMode(PIN_LED_SWITCH_1, OUTPUT);
  pinMode(PIN_LED_ROUTE, OUTPUT);
  pinMode(PIN_LED_LOCK, OUTPUT);

  // Attach the output pins
  pinMode(PIN_OUTPUT_SWITCH_1, OUTPUT);
  pinMode(PIN_OUTPUT_SWITCH_5, OUTPUT);

  // First make sure the output pins are OK
  update_outputs();

  // Attach pins to lever Bounce objects.
  levers[SWITCH_5].attach(PIN_LEVER_SWITCH_5);
  levers[SWITCH_1].attach(PIN_LEVER_SWITCH_1);
  levers[LOCK].attach(PIN_LOCK);
  levers[ROUTE_1].attach(PIN_LEVER_ROUTE_1);
  levers[ROUTE_2].attach(PIN_LEVER_ROUTE_2);

  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].interval(200);
  }

  // look up the initial lever positions and update accordingly.
  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].update();
    if (levers[number].read() == LOW) {
      react_to_movement(number, ACTUATED);
    }
    else {
      react_to_movement(number, REST);
    }
    delay(250);
  }

}


void loop() {
  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].update();
    if (levers[number].fell()) {
      react_to_movement(number, ACTUATED);
    }
    if (levers[number].rose()) {
      react_to_movement(number, REST);
    }
  }
  // Update the LEDs.
  update_single_lever_led(PIN_LED_SWITCH_5, SWITCH_5);
  update_single_lever_led(PIN_LED_SWITCH_1, SWITCH_1);
  update_single_lever_led(PIN_LED_LOCK, LOCK);

  update_dual_lever_led(PIN_LED_ROUTE, ROUTE_1, ROUTE_2);

}
