// #include <servomover.h>  /* Only for library detection */
// #include <Servo.h>  /* Only for library detection */
#include <Bounce2.h>

// Red LEDs named after the signal/switch lever/etc they're for.
// These are the analog ports labeled as a8-a15.
int PIN_LED_SWITCH_5 = A8;
int PIN_LED_SWITCH_1 = A9;
int PIN_LED_SIGNAL_A1 = A10;
int PIN_LED_SIGNAL_A2 = A11;
int PIN_LED_SWITCHING_PERMISSION = A12;
int PIN_LED_ROUTE_A = A13;
int PIN_LED_ROUTE_P = A14;
// White LED
int PIN_LED_ROUTE_AP_FIXATION = A15;
// Blue LED in "route recall" box
int PIN_LED_ROUTE_AP_RECALL = 49;

// Levers and lock
int PIN_LEVER_SWITCH_5 = 22;
int PIN_LEVER_SWITCH_1 = 24;
int PIN_LEVER_SIGNAL_A1 = 26;
int PIN_LEVER_SIGNAL_A2 = 28;
int PIN_LOCK_SWITCHING_PERMISSION = 32;
// Route levers
int PIN_LEVER_ROUTE_A1 = 36;
int PIN_LEVER_ROUTE_A2 = 38;
int PIN_LEVER_ROUTE_P1 = 40;
int PIN_LEVER_ROUTE_P2 = 42;
// Push button for route fixing
int PIN_BUTTON_ROUTE_AP_FIXATION = 46;
// Switch/lock for route recall
int PIN_LOCK_ROUTE_AP_RECALL = 47;

// Output pins (pull to 0 means go out of the ground position)
int PIN_OUTPUT_SWITCH_1 = A1;
int PIN_OUTPUT_SWITCH_5 =A2;
int PIN_OUTPUT_SIGNAL_A1 = A5;
int PIN_OUTPUT_SIGNAL_A2 = A4;


// Array index numbers. These are columns in the locking table.
int SWITCH_5 = 0;
int SWITCH_1 = 1;
int SWITCHING_PERMISSION = 2;  // Note: PLUS means 'key in lock'.
int ROUTE_A1 = 3;
int ROUTE_A2 = 4;
int ROUTE_P1 = 5;
int ROUTE_P2 = 6;
int ROUTE_AP_FIXATION = 7;
int SIGNAL_A1 = 8;
int SIGNAL_A2 = 9;
int ROUTE_AP_RECALL = 10;

// The two possible positions. Plus is the default, minus the actuated state.
bool PLUS = true;
bool MINUS = false;

// The states. All arrays with length 10.
const int ARRAY_SIZE = 11;
boolean current_position [ARRAY_SIZE] = { PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS , PLUS };
boolean blocked [ARRAY_SIZE] = { false, false, false, false, false, false, false, false, false, false, false};
boolean requirements_fulfilled [ARRAY_SIZE] = { true, true, true, false, false, false, false, false, false, false, false};
boolean position_is_ok [ARRAY_SIZE] = { true, true, true, true, true, true, true, true, true, true, true };


// Actual levers/buttons/locks.
Bounce levers [ARRAY_SIZE];


boolean movement_allowed (int number) {
  return (blocked[number] == false && requirements_fulfilled[number] == true);
}


void update_requirements() {
  // Requirements flow "upwards". The correct switch positions can enable
  // routes. Enabled routes plus an enabled route fixation can enable signals.

  // Requirements fulfilled by the switches.
  requirements_fulfilled[ROUTE_A1] = false;
  requirements_fulfilled[ROUTE_A2] = false;
  requirements_fulfilled[ROUTE_P1] = false;
  requirements_fulfilled[ROUTE_P2] = false;
  if (current_position[SWITCH_5] == PLUS &&
      current_position[SWITCH_1] == PLUS &&
      current_position[SWITCHING_PERMISSION] == PLUS) {
    requirements_fulfilled[ROUTE_A1] = true;
  }
  if (current_position[SWITCH_5] == MINUS &&
      current_position[SWITCH_1] == MINUS &&
      current_position[SWITCHING_PERMISSION] == PLUS) {
    requirements_fulfilled[ROUTE_A2] = true;
  }
  if (current_position[SWITCH_1] == PLUS &&
      current_position[SWITCHING_PERMISSION] == PLUS) {
    requirements_fulfilled[ROUTE_P1] = true;
  }
  if (current_position[SWITCH_1] == MINUS &&
      current_position[SWITCHING_PERMISSION] == PLUS) {
    requirements_fulfilled[ROUTE_P2] = true;
  }

  // If a route switch is MINUS, it can be fixated.
  requirements_fulfilled[ROUTE_AP_FIXATION] = false;
  if (current_position[ROUTE_A1] == MINUS ||
      current_position[ROUTE_A2] == MINUS ||
      current_position[ROUTE_P1] == MINUS ||
      current_position[ROUTE_P2] == MINUS) {
    requirements_fulfilled[ROUTE_AP_FIXATION] = true;
  }

}


void update_blocks() {
  // Blocks flow "downwards". A route lever blocks switches. A route fixation
  // blocks a route lever.

  // Reset all blocks.
  for (int number = 0; number < ARRAY_SIZE; number++) {
    blocked[number] = false;
  }

  // Route levers block each other. And they block switches.
  if (current_position[ROUTE_A1] == MINUS) {
    blocked[ROUTE_A2] = true;
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P2] = true;

    blocked[SWITCH_5] = true;
    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }
  if (current_position[ROUTE_A2] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P2] = true;

    blocked[SWITCH_5] = true;
    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }
  if (current_position[ROUTE_P1] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_A2] = true;
    blocked[ROUTE_P2] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }
  if (current_position[ROUTE_P2] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_A2] = true;
    blocked[ROUTE_P1] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }

  // Route fixation blocks routes.
  if (current_position[ROUTE_AP_FIXATION] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_A2] = true;
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P2] = true;
    // Special case: also keep yourself blocked (it is only un-blocked by
    // moving a signal back to PLUS).
    blocked[ROUTE_AP_FIXATION] = true;
  }

  // Signals also block routes.
  if (current_position[SIGNAL_A1] == MINUS || current_position[SIGNAL_A2] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_A2] = true;
  }
}


void update_outputs() {
  // Active: pull to zero.
  if (current_position[SWITCH_1] == PLUS) {
    digitalWrite(PIN_OUTPUT_SWITCH_1, HIGH);
  }
  else {
    digitalWrite(PIN_OUTPUT_SWITCH_1, LOW);
  }
  if (current_position[SWITCH_5] == PLUS) {
    digitalWrite(PIN_OUTPUT_SWITCH_5, HIGH);
  }
  else {
    digitalWrite(PIN_OUTPUT_SWITCH_5, LOW);
  }
  if (current_position[SIGNAL_A1] == PLUS) {
    digitalWrite(PIN_OUTPUT_SIGNAL_A1, HIGH);
  }
  else {
    digitalWrite(PIN_OUTPUT_SIGNAL_A1, LOW);
  }
  if (current_position[SIGNAL_A2] == PLUS) {
    digitalWrite(PIN_OUTPUT_SIGNAL_A2, HIGH);
  }
  else {
    digitalWrite(PIN_OUTPUT_SIGNAL_A2, LOW);
  }
}


void change_position(int number,
                     boolean new_position) {

  current_position[number] = new_position;
  position_is_ok[number] = true;

  update_requirements();
  update_blocks();
  update_outputs();

  // Special case: fixating a route enables the corresponding signal. This way
  // it can be thrown once (instead of through normal requirements, then it
  // would have been possible to throw it multiple times). It also enables the
  // corresponding route recall.
  if (number == ROUTE_AP_FIXATION && new_position == MINUS) {
    requirements_fulfilled[ROUTE_AP_RECALL] = true;
    if (current_position[ROUTE_A1] == MINUS) {
      requirements_fulfilled[SIGNAL_A1] = true;
    }
    if (current_position[ROUTE_A2] == MINUS) {
      requirements_fulfilled[SIGNAL_A2] = true;
    }
  }

  // Special case: route recall frees the route fixation.
  if (number == ROUTE_AP_RECALL) {
    // Any new position is OK, we don't care which way the lock is turned.
    change_position(ROUTE_AP_FIXATION, PLUS);
  }

  // Special case: signals going back to PLUS remove their own "requirements
  // fulfilled" marker.
  if (number == SIGNAL_A1 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_A1] = false;
  }
  if (number == SIGNAL_A2 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_A2] = false;
  }

}


void react_to_movement(int number,
                       boolean new_position) {
  if (movement_allowed(number)) {
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
      digitalWrite(pin, LOW);
    }
    else {
      digitalWrite(pin, HIGH);
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
      digitalWrite(pin, LOW);
    }
    else {
      digitalWrite(pin, HIGH);
    }
  }

}


void setup() {

  // Attach the pins and hook up react_to_movement() functions.
  // for (int number = 0; number < ARRAY_SIZE; number++) {
  //   levers[number] = Bounce();
  // }

  // Define lever pin modes.
  pinMode(PIN_LEVER_SWITCH_5, INPUT_PULLUP);
  pinMode(PIN_LEVER_SWITCH_1, INPUT_PULLUP);
  pinMode(PIN_LOCK_SWITCHING_PERMISSION, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_A1, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_A2, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_P1, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_P2, INPUT_PULLUP);
  pinMode(PIN_LEVER_SIGNAL_A1, INPUT_PULLUP);
  pinMode(PIN_LEVER_SIGNAL_A2, INPUT_PULLUP);
  pinMode(PIN_BUTTON_ROUTE_AP_FIXATION, INPUT_PULLUP);
  pinMode(PIN_LOCK_ROUTE_AP_RECALL, INPUT_PULLUP);

  // Attach the LEDs.
  pinMode(13, OUTPUT);
  pinMode(PIN_LED_SWITCH_5, OUTPUT);
  pinMode(PIN_LED_SWITCH_1, OUTPUT);
  pinMode(PIN_LED_SIGNAL_A2, OUTPUT);
  pinMode(PIN_LED_SIGNAL_A1, OUTPUT);
  pinMode(PIN_LED_SWITCHING_PERMISSION, OUTPUT);
  pinMode(PIN_LED_ROUTE_A, OUTPUT);
  pinMode(PIN_LED_ROUTE_P, OUTPUT);
  pinMode(PIN_LED_ROUTE_AP_FIXATION, OUTPUT);
  pinMode(PIN_LED_ROUTE_AP_RECALL, OUTPUT);

  // Attach the output pins
  pinMode(PIN_OUTPUT_SWITCH_1, OUTPUT);
  pinMode(PIN_OUTPUT_SWITCH_5, OUTPUT);
  pinMode(PIN_OUTPUT_SIGNAL_A1, OUTPUT);
  pinMode(PIN_OUTPUT_SIGNAL_A2, OUTPUT);

  // Debug led 13 stuff
  digitalWrite(13, LOW);

  // First make sure the output pins are OK
  update_outputs();

  // Attach pins to lever Bounce objects.
  levers[SWITCH_5].attach(PIN_LEVER_SWITCH_5);
  levers[SWITCH_1].attach(PIN_LEVER_SWITCH_1);
  levers[SWITCHING_PERMISSION].attach(PIN_LOCK_SWITCHING_PERMISSION);
  levers[ROUTE_A1].attach(PIN_LEVER_ROUTE_A1);
  levers[ROUTE_A2].attach(PIN_LEVER_ROUTE_A2);
  levers[ROUTE_P1].attach(PIN_LEVER_ROUTE_P1);
  levers[ROUTE_P2].attach(PIN_LEVER_ROUTE_P2);
  levers[SIGNAL_A1].attach(PIN_LEVER_SIGNAL_A1);
  levers[SIGNAL_A2].attach(PIN_LEVER_SIGNAL_A2);
  levers[ROUTE_AP_FIXATION].attach(PIN_BUTTON_ROUTE_AP_FIXATION);
  levers[ROUTE_AP_RECALL].attach(PIN_LOCK_ROUTE_AP_RECALL);

  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].interval(200);
  }
  // Special case: route fixation button should take some seconds to activate.
  levers[ROUTE_AP_FIXATION].interval(2000);

  // look up the initial lever positions and update accordingly.
  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].update();

    if (levers[number].read() == HIGH) {
      react_to_movement(number, PLUS);
    }
    else {
      react_to_movement(number, MINUS);
    }
  }

}


void loop() {
  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].update();
    if (levers[number].rose()) {
      react_to_movement(number, PLUS);
    }
    if (levers[number].fell()) {
      react_to_movement(number, MINUS);
    }
  }
  // Update the LEDs.
  update_single_lever_led(PIN_LED_SWITCH_5, SWITCH_5);
  update_single_lever_led(PIN_LED_SWITCH_1, SWITCH_1);
  update_single_lever_led(PIN_LED_SIGNAL_A1, SIGNAL_A1);
  update_single_lever_led(PIN_LED_SIGNAL_A2, SIGNAL_A2);
  update_single_lever_led(PIN_LED_SWITCHING_PERMISSION, SWITCHING_PERMISSION);

  update_dual_lever_led(PIN_LED_ROUTE_A, ROUTE_A1, ROUTE_A2);
  update_dual_lever_led(PIN_LED_ROUTE_P, ROUTE_P1, ROUTE_P2);

  if (current_position[ROUTE_AP_FIXATION] == PLUS) {
    digitalWrite(PIN_LED_ROUTE_AP_FIXATION, LOW);
    digitalWrite(PIN_LED_ROUTE_AP_RECALL, LOW);
  }
  else {
    digitalWrite(PIN_LED_ROUTE_AP_FIXATION, HIGH);
    digitalWrite(PIN_LED_ROUTE_AP_RECALL, HIGH);
  }

  // TODO: update the outgoing pins.
}
