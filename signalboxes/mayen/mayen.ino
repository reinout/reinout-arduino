#include <Bounce2.h>
#include "Waveshare_LCD1602.h"

// The two possible positions. Plus is the default, minus the actuated state.
bool PLUS = true;
bool MINUS = false;

// Debug stuff
int PIN_BOARD_LED = 13;

// Relais pins. Switch x also means the relais for the polarisation for switch x.
// Pulling to 0 means the non-default "minus" position for switches.
int PIN_OUTPUT_SWITCH_1 = 2;
int PIN_OUTPUT_SWITCH_2 = 3;
int PIN_OUTPUT_SWITCH_3 = 4;
int PIN_OUTPUT_SWITCH_4 = 5;
int PIN_OUTPUT_SWITCH_5 = 6;
int PIN_OUTPUT_SWITCH_6 = 7;

// Signal pins. Pulling to 0 means the non-default "minus" position, so "green" instead
// of "red.
int PIN_OUTPUT_SIGNAL_P1 = 8;
int PIN_OUTPUT_SIGNAL_P2 = 9;
int PIN_OUTPUT_SIGNAL_P3 = 10;
int PIN_OUTPUT_SIGNAL_P4 = 11;
// TODO: G1/2 signal (not relais, though).

// Lever pins
int PIN_LEVER_SWITCH_1 = 22;
int PIN_LEVER_SWITCH_2 = 23;
int PIN_LEVER_SWITCH_3 = 24;
int PIN_LEVER_SWITCH_4 = 25;
int PIN_LEVER_SWITCH_5 = 26;
int PIN_LEVER_SWITCH_6 = 27;
// TODO: signal levers
int PIN_LEVER_SIGNAL_G1 = 33;
int PIN_LEVER_SIGNAL_G2 = 32;
int PIN_LEVER_SIGNAL_P1 = 31;
int PIN_LEVER_SIGNAL_P2 = 30;
int PIN_LEVER_SIGNAL_P3 = 29;
int PIN_LEVER_SIGNAL_P4 = 28;

// TODO: route selector levers
int PIN_LEVER_ROUTE_G1 = 46;
int PIN_LEVER_ROUTE_G2 = 47;
int PIN_LEVER_ROUTE_G3 = 48;
int PIN_LEVER_ROUTE_G4 = 49;
int PIN_LEVER_ROUTE_P1 = 50;
int PIN_LEVER_ROUTE_P2 = 51;
int PIN_LEVER_ROUTE_P3 = 52;
int PIN_LEVER_ROUTE_P4 = 53;

// TODO: PIN_LED_SWITCH_etc
int PIN_LED_SWITCH_1 = 39;
int PIN_LED_SWITCH_2 = 38;
int PIN_LED_SWITCH_3 = 37;
int PIN_LED_SWITCH_4 = 36;
int PIN_LED_SWITCH_5 = 35;
int PIN_LED_SWITCH_6 = 34;

int PIN_LED_SIGNAL_G1 = 45;
int PIN_LED_SIGNAL_G2 = 44;
int PIN_LED_SIGNAL_P1 = 43;
int PIN_LED_SIGNAL_P2 = 42;
int PIN_LED_SIGNAL_P3 = 41;
int PIN_LED_SIGNAL_P4 = 40;

int PIN_LED_ROUTE_1 = A15;
int PIN_LED_ROUTE_2 = A14;
int PIN_LED_ROUTE_3 = A13;
int PIN_LED_ROUTE_4 = A12;

int PIN_LED_ROUTE_FIXATION = A10;

// PINs for extra thingies
int PIN_TRACK_CONTACT = A0;
int PIN_BUTTON_ROUTE_FIXATION = A11;

// Display, 16 characters, 2 lines.
Waveshare_LCD1602 display(16,2);

// Array index numbers. These are columns in the locking table.
int TRACK_CONTACT = 0;
int SWITCH_1 = 1;
int SWITCH_2 = 2;
int SWITCH_3 = 3;
int SWITCH_4 = 4;
int SWITCH_5 = 5;
int SWITCH_6 = 6;
int SIGNAL_P1 = 7;
int SIGNAL_P2 = 8;
int SIGNAL_P3 = 9;
int SIGNAL_P4 = 10;
int SIGNAL_G1 = 11;
int SIGNAL_G2 = 12;
int ROUTE_G1 = 13;
int ROUTE_G2 = 14;
int ROUTE_G3 = 15;
int ROUTE_G4 = 16;
int ROUTE_P1 = 17;
int ROUTE_P2 = 18;
int ROUTE_P3 = 19;
int ROUTE_P4 = 20;
int ROUTE_FIXATION = 21;
int ROUTE_414 = 22;
int ROUTE_514 = 23;
int ROUTE_415 = 24;
int ROUTE_515 = 25;
int ROUTE_IN5 = 26;  // Rangieren in <-> 5

// The states. All arrays with length 18.
const int ARRAY_SIZE = 27;
char name[] [ARRAY_SIZE] = {
  "Contact",  // Route recall
  "W1",  // Switch 1
  "W2",
  "W3",
  "W4",
  "W5",
  "W6",
  "P1",  // Signal P1
  "P2",
  "P3",
  "P4",
  "G1",  // Signal G1
  "G2",
  "f g1",  // Route G1
  "f g2",
  "f g3",
  "f g4",
  "f p1",  // Route P1
  "f p2",
  "f p3",
  "f p4",
  "Fix",  // Route fixation
  "r 4-14", // Rangieren 4 - 14
  "r 5-14",
  "r 4-15",
  "r 5-15",
  "r in-5",  // Rangieren in - 5
};

boolean current_position [ARRAY_SIZE] = {
  PLUS,  // Route recall, plus=rest, minus=train is passing
  PLUS,  // Switch 1
  PLUS,
  PLUS,
  PLUS,
  PLUS,
  PLUS,
  PLUS,  // Signal P1
  PLUS,
  PLUS,
  PLUS,
  PLUS,  // Signal G1
  PLUS,
  PLUS,  // Route G1
  PLUS,
  PLUS,
  PLUS,
  PLUS,  // Route P1
  PLUS,
  PLUS,
  PLUS,
  PLUS,  // Route fixation
  PLUS,  // Rangieren 4-14
  PLUS,
  PLUS,
  PLUS,
  PLUS,
};

boolean blocked [ARRAY_SIZE] = {
  false,  // Route recall
  false,  // Switch 1
  false,
  false,
  false,
  false,
  false,
  false,  // Signal P1
  false,
  false,
  false,
  false,  // Signal G1
  false,
  false,  // Route G1
  false,
  false,
  false,
  false,  // Route P1
  false,
  false,
  false,
  false,  // Route fixation
  false,  // Rangieren 4-14
  false,
  false,
  false,
  false,
};

boolean requirements_fulfilled [ARRAY_SIZE] = {
  true,  // Route recall  TODO set to false
  true,  // Switch 1
  true,
  true,
  true,
  true,
  true,
  false,  // Signal P1
  false,
  false,
  false,
  false,  // Signal G1
  false,
  false,  // Route G1
  false,
  false,
  false,
  false,  // Route P1
  false,
  false,
  false,
  false,  // Route fixation
  false,  // Rangieren 4-14
  false,
  false,
  false,
  false,
};

boolean position_is_ok [ARRAY_SIZE] = {
  true,  // Route recall
  true,  // Switch 1
  true,
  true,
  true,
  true,
  true,
  true,  // Signal P1
  true,
  true,
  true,
  true,  // Signal G1
  true,
  true,  // Route G1
  true,
  true,
  true,
  true,  // Route P1
  true,
  true,
  true,
  true,  // Route fixation
  true,  // Rangieren 4-14
  true,
  true,
  true,
  true,
};

// Actual levers/buttons/locks.
Bounce levers [ARRAY_SIZE];


void update_output(int pin, int array_position) {
  // Active: pull to zero.
  if (current_position[array_position] == PLUS) {
    digitalWrite(pin, HIGH);
  }
  else {
    digitalWrite(pin, LOW);
  }
}


void update_outputs() {
  update_output(PIN_OUTPUT_SWITCH_1, SWITCH_1);
  update_output(PIN_OUTPUT_SWITCH_2, SWITCH_2);
  update_output(PIN_OUTPUT_SWITCH_3, SWITCH_3);
  update_output(PIN_OUTPUT_SWITCH_4, SWITCH_4);
  update_output(PIN_OUTPUT_SWITCH_5, SWITCH_5);
  update_output(PIN_OUTPUT_SWITCH_6, SWITCH_6);
  update_output(PIN_OUTPUT_SIGNAL_P1, SIGNAL_P1);
  update_output(PIN_OUTPUT_SIGNAL_P2, SIGNAL_P2);
  update_output(PIN_OUTPUT_SIGNAL_P3, SIGNAL_P3);
  update_output(PIN_OUTPUT_SIGNAL_P4, SIGNAL_P4);
  // TODO: G1/2
  update_output(PIN_LED_ROUTE_FIXATION, ROUTE_FIXATION);
}


boolean movement_allowed (int number) {
  return (blocked[number] == false && requirements_fulfilled[number] == true);
}

void update_requirements() {
  // Requirements flow "upwards". The correct switch positions can enable
  // routes. Enabled routes plus an enabled route fixation can enable signals.

  // Requirements fulfilled by the switches.
  requirements_fulfilled[ROUTE_G1] = false;
  requirements_fulfilled[ROUTE_G2] = false;
  requirements_fulfilled[ROUTE_G3] = false;
  requirements_fulfilled[ROUTE_G4] = false;
  requirements_fulfilled[ROUTE_P1] = false;
  requirements_fulfilled[ROUTE_P2] = false;
  requirements_fulfilled[ROUTE_P3] = false;
  requirements_fulfilled[ROUTE_P4] = false;
  requirements_fulfilled[ROUTE_414] = false;
  requirements_fulfilled[ROUTE_415] = false;
  requirements_fulfilled[ROUTE_514] = false;
  requirements_fulfilled[ROUTE_515] = false;
  requirements_fulfilled[ROUTE_IN5] = false;
  requirements_fulfilled[ROUTE_FIXATION] = false;

  if (current_position[SWITCH_1] == MINUS &&
      current_position[SWITCH_3] == MINUS &&
      current_position[SWITCH_4] == PLUS) {
    requirements_fulfilled[ROUTE_G1] = true;
    requirements_fulfilled[ROUTE_P1] = true;
  }
  if (current_position[SWITCH_1] == MINUS &&
      current_position[SWITCH_3] == PLUS &&
      current_position[SWITCH_4] == PLUS) {
    requirements_fulfilled[ROUTE_G2] = true;
    requirements_fulfilled[ROUTE_P2] = true;
  }
  if (current_position[SWITCH_1] == PLUS &&
      current_position[SWITCH_2] == PLUS &&
      current_position[SWITCH_4] == PLUS) {
    requirements_fulfilled[ROUTE_G3] = true;
    requirements_fulfilled[ROUTE_P3] = true;
  }
  if (current_position[SWITCH_1] == PLUS &&
      current_position[SWITCH_2] == MINUS &&
      current_position[SWITCH_4] == MINUS &&
      current_position[SWITCH_5] == PLUS &&
      current_position[SWITCH_6] == PLUS) {
    requirements_fulfilled[ROUTE_G4] = true;
    requirements_fulfilled[ROUTE_P4] = true;
  }
  if (current_position[SWITCH_4] == PLUS &&
      current_position[SWITCH_5] == PLUS &&
      current_position[SWITCH_6] == PLUS) {
    requirements_fulfilled[ROUTE_414] = true;
  }
  if (current_position[SWITCH_4] == PLUS &&
      current_position[SWITCH_5] == MINUS &&
      current_position[SWITCH_6] == PLUS) {
    requirements_fulfilled[ROUTE_415] = true;
  }
  if (current_position[SWITCH_4] == PLUS &&
      current_position[SWITCH_5] == PLUS &&
      current_position[SWITCH_6] == MINUS) {
    requirements_fulfilled[ROUTE_514] = true;
  }
  if (current_position[SWITCH_4] == PLUS &&
      current_position[SWITCH_5] == MINUS &&
      current_position[SWITCH_6] == MINUS) {
    requirements_fulfilled[ROUTE_515] = true;
  }
  if (current_position[SWITCH_1] == PLUS &&
      current_position[SWITCH_2] == MINUS &&
      current_position[SWITCH_4] == MINUS &&
      current_position[SWITCH_5] == PLUS &&
      current_position[SWITCH_6] == MINUS) {
    requirements_fulfilled[ROUTE_IN5] = true;
  }

  // If a route switch is MINUS, it can be fixated.
  if (current_position[ROUTE_G1] == MINUS ||
      current_position[ROUTE_G2] == MINUS ||
      current_position[ROUTE_G3] == MINUS ||
      current_position[ROUTE_G4] == MINUS ||
      current_position[ROUTE_P1] == MINUS ||
      current_position[ROUTE_P2] == MINUS ||
      current_position[ROUTE_P3] == MINUS ||
      current_position[ROUTE_P4] == MINUS) {
    requirements_fulfilled[ROUTE_FIXATION] = true;
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
  if (current_position[ROUTE_P1] == MINUS || current_position[ROUTE_G1] == MINUS ) {
    blocked[ROUTE_P2] = true;
    blocked[ROUTE_P3] = true;
    blocked[ROUTE_P4] = true;
    blocked[ROUTE_G2] = true;
    blocked[ROUTE_G3] = true;
    blocked[ROUTE_G4] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCH_3] = true;
    blocked[SWITCH_4] = true;
  }

  if (current_position[ROUTE_P2] == MINUS || current_position[ROUTE_G2] == MINUS ) {
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P3] = true;
    blocked[ROUTE_P4] = true;
    blocked[ROUTE_G1] = true;
    blocked[ROUTE_G3] = true;
    blocked[ROUTE_G4] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCH_3] = true;
    blocked[SWITCH_4] = true;
  }
  if (current_position[ROUTE_P3] == MINUS || current_position[ROUTE_G3] == MINUS ) {
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P2] = true;
    blocked[ROUTE_P4] = true;
    blocked[ROUTE_G1] = true;
    blocked[ROUTE_G2] = true;
    blocked[ROUTE_G4] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCH_2] = true;
    blocked[SWITCH_4] = true;
  }
  if (current_position[ROUTE_P4] == MINUS || current_position[ROUTE_G4] == MINUS ) {
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P2] = true;
    blocked[ROUTE_P3] = true;
    blocked[ROUTE_G1] = true;
    blocked[ROUTE_G2] = true;
    blocked[ROUTE_G3] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCH_2] = true;
    blocked[SWITCH_4] = true;
    blocked[SWITCH_5] = true;
    blocked[SWITCH_6] = true;
  }

  // Route fixation blocks routes.
  if (current_position[ROUTE_FIXATION] == MINUS) {
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P2] = true;
    blocked[ROUTE_P4] = true;
    blocked[ROUTE_P4] = true;
    blocked[ROUTE_G1] = true;
    blocked[ROUTE_G2] = true;
    blocked[ROUTE_G4] = true;
    blocked[ROUTE_G4] = true;
    // Special case: also keep yourself blocked (it is only un-blocked by
    // moving a signal back to PLUS).
    // TODO: state is not really retained?
    blocked[ROUTE_FIXATION] = true;
  }

  // Only one signal can be active, so any signal whatsoever blocks all routes.
  if (current_position[SIGNAL_P1] == MINUS ||
      current_position[SIGNAL_P2] == MINUS ||
      current_position[SIGNAL_P3] == MINUS ||
      current_position[SIGNAL_P4] == MINUS ||
      current_position[SIGNAL_G1] == MINUS ||
      current_position[SIGNAL_G2] == MINUS) {
    blocked[ROUTE_P1] = true;
    blocked[ROUTE_P2] = true;
    blocked[ROUTE_P3] = true;
    blocked[ROUTE_P4] = true;
    blocked[ROUTE_G1] = true;
    blocked[ROUTE_G2] = true;
  }

}

void write_first_line() {
  display.setCursor(9,0);
  display.send_string("        ");
  display.setCursor(9,0);
  if (requirements_fulfilled[ROUTE_414]) {
    display.send_string("14 -- 4");
  }
  if (requirements_fulfilled[ROUTE_415]) {
    display.send_string("15 -- 4");
  }
  if (requirements_fulfilled[ROUTE_514]) {
    display.send_string("14 -- 5");
  }
  if (requirements_fulfilled[ROUTE_515]) {
    display.send_string("15 -- 5");
  }
  if (requirements_fulfilled[ROUTE_IN5]) {
    display.send_string("in -- 5");
  }
}

void write_second_line() {
  char* msg;
  char* arrow;
  char* track;
  arrow = " -- ";

  display.setCursor(0,1);
  display.send_string("          ");

  display.setCursor(0,1);
  if (requirements_fulfilled[ROUTE_G1]) {
    track = "1";
  }
  if (requirements_fulfilled[ROUTE_G2]) {
    track = "2";
  }
  if (requirements_fulfilled[ROUTE_G3]) {
    track = "3";
  }
  if (requirements_fulfilled[ROUTE_G4]) {
    track = "4";
  }

  if (current_position[ROUTE_P1] == MINUS ||
      current_position[ROUTE_P2] == MINUS ||
      current_position[ROUTE_P3] == MINUS ||
      current_position[ROUTE_P4] == MINUS) {
    if (current_position[ROUTE_FIXATION] == MINUS) {
      arrow = "<== ";
    }
    else {
      arrow = "<-- ";
    }
  }
  if (current_position[ROUTE_G1] == MINUS ||
      current_position[ROUTE_G2] == MINUS ||
      current_position[ROUTE_G3] == MINUS ||
      current_position[ROUTE_G4] == MINUS) {
    if (current_position[ROUTE_FIXATION] == MINUS) {
      arrow = " ==>";
    }
    else {
      arrow = " -->";
    }
  }

  sprintf(msg, "in%s%s", arrow, track);
  display.send_string(msg);
}

void write_debug_info(int number) {
  display.setCursor(0,0);
  display.send_string("         ");
  display.setCursor(0,0);
  display.send_string(name[number]);
}

void change_position(int number,
                     boolean new_position) {

  // write_debug_info(number);  // Debug stuff.

  current_position[number] = new_position;
  position_is_ok[number] = true;

  update_requirements();
  update_blocks();
  update_outputs();
  write_first_line();
  write_second_line();

  // Special case: fixating a route enables the corresponding signal. This way
  // it can be thrown once (instead of through normal requirements, then it
  // would have been possible to throw it multiple times). It also enables the
  // corresponding route recall.
  if (number == ROUTE_FIXATION && new_position == MINUS) {
    requirements_fulfilled[TRACK_CONTACT] = true;
    // TODO: probably activate it? Or increase its bounce value?
    if (current_position[ROUTE_P1] == MINUS) {
      requirements_fulfilled[SIGNAL_P1] = true;
    }
    if (current_position[ROUTE_P2] == MINUS) {
      requirements_fulfilled[SIGNAL_P2] = true;
    }
    if (current_position[ROUTE_P3] == MINUS) {
      requirements_fulfilled[SIGNAL_P3] = true;
    }
    if (current_position[ROUTE_P4] == MINUS) {
      requirements_fulfilled[SIGNAL_P4] = true;
    }
    if (current_position[ROUTE_G1] == MINUS) {
      requirements_fulfilled[SIGNAL_G2] = true;  // HP2
    }
    if (current_position[ROUTE_G2] == MINUS) {
      requirements_fulfilled[SIGNAL_G2] = true;  // HP2
    }
    if (current_position[ROUTE_G3] == MINUS) {
      requirements_fulfilled[SIGNAL_G1] = true;  // HP1
    }
    if (current_position[ROUTE_G4] == MINUS) {
      requirements_fulfilled[SIGNAL_G2] = true;  // HP2
    }
  }

  // Special case: track contact enabled? Increase the bounce value so that it takes its
  // time falling back to PLUS.
  if (number == TRACK_CONTACT && new_position == MINUS) {
    levers[TRACK_CONTACT].interval(4000);
  }
  // Special case: track contact frees the route fixation. And disables itself.
  if (number == TRACK_CONTACT && new_position == PLUS) {
    levers[TRACK_CONTACT].interval(100);  // Back to normal twitchy behaviour.
    change_position(ROUTE_FIXATION, PLUS);
    requirements_fulfilled[TRACK_CONTACT] = false;
  }

  // Special case: signals going back to PLUS remove their own "requirements
  // fulfilled" marker.
  if (number == SIGNAL_P1 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_P1] = false;
  }
  if (number == SIGNAL_P2 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_P2] = false;
  }
  if (number == SIGNAL_P3 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_P3] = false;
  }
  if (number == SIGNAL_P4 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_P4] = false;
  }
  if (number == SIGNAL_G1 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_G1] = false;
  }
  if (number == SIGNAL_G2 && new_position == PLUS) {
      requirements_fulfilled[SIGNAL_G2] = false;
  }

}


void react_to_movement(int number,
                       boolean new_position) {
  if (number == ROUTE_FIXATION && new_position == PLUS) {
    // Ignore this, we only want to know if the route fixation button was activated.
    // Deactivation happens via the track contact.
    return;
  }

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
      digitalWrite(pin, LOW);
    }
    else {
      digitalWrite(pin, HIGH);
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
      digitalWrite(pin, LOW);
    }
    else {
      digitalWrite(pin, HIGH);
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
  display.init();

  // Debug led
  pinMode(PIN_BOARD_LED, OUTPUT);

  // Attach the output pins
  pinMode(PIN_OUTPUT_SWITCH_1, OUTPUT);
  pinMode(PIN_OUTPUT_SWITCH_2, OUTPUT);
  pinMode(PIN_OUTPUT_SWITCH_3, OUTPUT);
  pinMode(PIN_OUTPUT_SWITCH_4, OUTPUT);
  pinMode(PIN_OUTPUT_SWITCH_5, OUTPUT);
  pinMode(PIN_OUTPUT_SWITCH_6, OUTPUT);
  pinMode(PIN_OUTPUT_SIGNAL_P1, OUTPUT);
  pinMode(PIN_OUTPUT_SIGNAL_P2, OUTPUT);
  pinMode(PIN_OUTPUT_SIGNAL_P3, OUTPUT);
  pinMode(PIN_OUTPUT_SIGNAL_P4, OUTPUT);
  // Attach the levers
  pinMode(PIN_LEVER_SWITCH_1, INPUT_PULLUP);
  pinMode(PIN_LEVER_SWITCH_2, INPUT_PULLUP);
  pinMode(PIN_LEVER_SWITCH_3, INPUT_PULLUP);
  pinMode(PIN_LEVER_SWITCH_4, INPUT_PULLUP);
  pinMode(PIN_LEVER_SWITCH_5, INPUT_PULLUP);
  pinMode(PIN_LEVER_SWITCH_6, INPUT_PULLUP);

  pinMode(PIN_LEVER_SIGNAL_G1, INPUT_PULLUP);
  pinMode(PIN_LEVER_SIGNAL_G2, INPUT_PULLUP);
  pinMode(PIN_LEVER_SIGNAL_P4, INPUT_PULLUP);
  pinMode(PIN_LEVER_SIGNAL_P3, INPUT_PULLUP);
  pinMode(PIN_LEVER_SIGNAL_P2, INPUT_PULLUP);
  pinMode(PIN_LEVER_SIGNAL_P1, INPUT_PULLUP);

  pinMode(PIN_LEVER_ROUTE_P1, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_P2, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_P3, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_P4, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_G1, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_G2, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_G3, INPUT_PULLUP);
  pinMode(PIN_LEVER_ROUTE_G4, INPUT_PULLUP);

  // Attach the extra stuff
  pinMode(PIN_TRACK_CONTACT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_ROUTE_FIXATION, INPUT_PULLUP);

  // Attach the LEDs
  pinMode(PIN_LED_SWITCH_1, OUTPUT);
  pinMode(PIN_LED_SWITCH_2, OUTPUT);
  pinMode(PIN_LED_SWITCH_3, OUTPUT);
  pinMode(PIN_LED_SWITCH_4, OUTPUT);
  pinMode(PIN_LED_SWITCH_5, OUTPUT);
  pinMode(PIN_LED_SWITCH_6, OUTPUT);

  pinMode(PIN_LED_SIGNAL_P1, OUTPUT);
  pinMode(PIN_LED_SIGNAL_P2, OUTPUT);
  pinMode(PIN_LED_SIGNAL_P3, OUTPUT);
  pinMode(PIN_LED_SIGNAL_P4, OUTPUT);
  pinMode(PIN_LED_SIGNAL_G1, OUTPUT);
  pinMode(PIN_LED_SIGNAL_G2, OUTPUT);

  pinMode(PIN_LED_ROUTE_1, OUTPUT);
  pinMode(PIN_LED_ROUTE_2, OUTPUT);
  pinMode(PIN_LED_ROUTE_3, OUTPUT);
  pinMode(PIN_LED_ROUTE_4, OUTPUT);

  pinMode(PIN_LED_ROUTE_FIXATION, OUTPUT);

  // First make sure the output pins are OK
  update_outputs();

  // Attach pins to lever Bounce objects.
  levers[SWITCH_1].attach(PIN_LEVER_SWITCH_1);
  levers[SWITCH_2].attach(PIN_LEVER_SWITCH_2);
  levers[SWITCH_3].attach(PIN_LEVER_SWITCH_3);
  levers[SWITCH_4].attach(PIN_LEVER_SWITCH_4);
  levers[SWITCH_5].attach(PIN_LEVER_SWITCH_5);
  levers[SWITCH_6].attach(PIN_LEVER_SWITCH_6);

  levers[SIGNAL_G1].attach(PIN_LEVER_SIGNAL_G1);
  levers[SIGNAL_G2].attach(PIN_LEVER_SIGNAL_G2);
  levers[SIGNAL_P1].attach(PIN_LEVER_SIGNAL_P1);
  levers[SIGNAL_P2].attach(PIN_LEVER_SIGNAL_P2);
  levers[SIGNAL_P3].attach(PIN_LEVER_SIGNAL_P3);
  levers[SIGNAL_P4].attach(PIN_LEVER_SIGNAL_P4);

  levers[ROUTE_P1].attach(PIN_LEVER_ROUTE_P1);
  levers[ROUTE_P2].attach(PIN_LEVER_ROUTE_P2);
  levers[ROUTE_P3].attach(PIN_LEVER_ROUTE_P3);
  levers[ROUTE_P4].attach(PIN_LEVER_ROUTE_P4);
  levers[ROUTE_G1].attach(PIN_LEVER_ROUTE_G1);
  levers[ROUTE_G2].attach(PIN_LEVER_ROUTE_G2);
  levers[ROUTE_G3].attach(PIN_LEVER_ROUTE_G3);
  levers[ROUTE_G4].attach(PIN_LEVER_ROUTE_G4);

  levers[TRACK_CONTACT].attach(PIN_TRACK_CONTACT);
  levers[ROUTE_FIXATION].attach(PIN_BUTTON_ROUTE_FIXATION);

  // Set bounce behavior.
  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].interval(200);
  }
  // Special case: route fixation button should take two second to activate.
  levers[ROUTE_FIXATION].interval(2000);
  // And the track contact should have really twitchy behaviour.
  levers[TRACK_CONTACT].interval(100);

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
  update_single_lever_led(PIN_LED_SWITCH_1, SWITCH_1);
  update_single_lever_led(PIN_LED_SWITCH_2, SWITCH_2);
  update_single_lever_led(PIN_LED_SWITCH_3, SWITCH_3);
  update_single_lever_led(PIN_LED_SWITCH_4, SWITCH_4);
  update_single_lever_led(PIN_LED_SWITCH_5, SWITCH_5);
  update_single_lever_led(PIN_LED_SWITCH_6, SWITCH_6);

  update_single_lever_led(PIN_LED_SIGNAL_P1, SIGNAL_P1);
  update_single_lever_led(PIN_LED_SIGNAL_P2, SIGNAL_P2);
  update_single_lever_led(PIN_LED_SIGNAL_P3, SIGNAL_P3);
  update_single_lever_led(PIN_LED_SIGNAL_P4, SIGNAL_P4);
  update_single_lever_led(PIN_LED_SIGNAL_G1, SIGNAL_G1);
  update_single_lever_led(PIN_LED_SIGNAL_G2, SIGNAL_G2);

  update_dual_lever_led(PIN_LED_ROUTE_1, ROUTE_P1, ROUTE_G1);
  update_dual_lever_led(PIN_LED_ROUTE_2, ROUTE_P2, ROUTE_G2);
  update_dual_lever_led(PIN_LED_ROUTE_3, ROUTE_P3, ROUTE_G3);
  update_dual_lever_led(PIN_LED_ROUTE_4, ROUTE_P4, ROUTE_G4);

}
