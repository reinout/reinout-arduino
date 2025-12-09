#include <Bounce2.h>
#include "Waveshare_LCD1602.h"

// The two possible positions. Plus is the default, minus the actuated state.
bool PLUS = true;
bool MINUS = false;

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
// TODO: F1/2 signal (not relais, though).

// Lever pins
int PIN_LEVER_SWITCH_1 = 22;
int PIN_LEVER_SWITCH_2 = 23;
int PIN_LEVER_SWITCH_3 = 24;
int PIN_LEVER_SWITCH_4 = 25;
int PIN_LEVER_SWITCH_5 = 26;
int PIN_LEVER_SWITCH_6 = 27;
// TODO: signal levers


// Display, 16 characters, 2 lines.
Waveshare_LCD1602 display(16,2);

// Array index numbers. These are columns in the locking table.
int ROUTE_RECALL = 0;
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
int ROUTE_IN5 = 26;  // Rangier in <-> 5

// The states. All arrays with length 18.
const int ARRAY_SIZE = 27;
char *name[] [ARRAY_SIZE] = {
  "Gleiskontakt",  // Route recall
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
  "Festlegen",  // Route fixation
  "r 4-14", // Rangieren 4 - 14
  "r 5-14", // Rangieren 4 - 14
  "r 4-15", // Rangieren 4 - 14
  "r 5-15", // Rangieren 4 - 14
  "r in-5",  // Rangieren in - 5
};

boolean current_position [ARRAY_SIZE] = {
  PLUS,  // Route recall
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
  false,  // Route recall
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


}

void update_blocks() {}

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
  display.setCursor(0,1);
  display.send_string("        ");
  display.setCursor(0,1);
  if (requirements_fulfilled[ROUTE_G1]) {
    display.send_string("in -- 1");
  }
  if (requirements_fulfilled[ROUTE_G2]) {
    display.send_string("in -- 2");
  }
  if (requirements_fulfilled[ROUTE_G3]) {
    display.send_string("in -- 3");
  }
  if (requirements_fulfilled[ROUTE_G4]) {
    display.send_string("in -- 4");
  }
  // TODO: if position[ROUTE_G1/P1], then '<--'. Locked? '<=='
}

void change_position(int number,
                     boolean new_position) {

  current_position[number] = new_position;
  position_is_ok[number] = true;

  update_requirements();
  update_blocks();
  update_outputs();
  write_first_line();
  write_second_line();

  /* // Special case: fixating a route enables the corresponding signal. This way */
  /* // it can be thrown once (instead of through normal requirements, then it */
  /* // would have been possible to throw it multiple times). It also enables the */
  /* // corresponding route recall. */
  /* if (number == ROUTE_AP_FIXATION && new_position == MINUS) { */
  /*   requirements_fulfilled[ROUTE_AP_RECALL] = true; */
  /*   if (current_position[ROUTE_A1] == MINUS) { */
  /*     requirements_fulfilled[SIGNAL_A1] = true; */
  /*   } */
  /*   if (current_position[ROUTE_A2] == MINUS) { */
  /*     requirements_fulfilled[SIGNAL_A2] = true; */
  /*   } */
  /* } */

  /* // Special case: route recall frees the route fixation. */
  /* if (number == ROUTE_AP_RECALL) { */
  /*   // Any new position is OK, we don't care which way the lock is turned. */
  /*   change_position(ROUTE_AP_FIXATION, PLUS); */
  /* } */

  /* // Special case: signals going back to PLUS remove their own "requirements */
  /* // fulfilled" marker. */
  /* if (number == SIGNAL_A1 && new_position == PLUS) { */
  /*     requirements_fulfilled[SIGNAL_A1] = false; */
  /* } */
  /* if (number == SIGNAL_A2 && new_position == PLUS) { */
  /*     requirements_fulfilled[SIGNAL_A2] = false; */
  /* } */

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



void setup() {
  display.init();

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

  // First make sure the output pins are OK
  update_outputs();

  // Attach pins to lever Bounce objects.
  levers[SWITCH_1].attach(PIN_LEVER_SWITCH_1);
  levers[SWITCH_2].attach(PIN_LEVER_SWITCH_2);
  levers[SWITCH_3].attach(PIN_LEVER_SWITCH_3);
  levers[SWITCH_4].attach(PIN_LEVER_SWITCH_4);
  levers[SWITCH_5].attach(PIN_LEVER_SWITCH_5);
  levers[SWITCH_6].attach(PIN_LEVER_SWITCH_6);
  // Set bounce behavior.
  for (int number = 0; number < ARRAY_SIZE; number++) {
    levers[number].interval(200);
  }
  // TODO Special case: route fixation button should take some seconds to activate.
  // levers[ROUTE_AP_FIXATION].interval(2000);

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
}
