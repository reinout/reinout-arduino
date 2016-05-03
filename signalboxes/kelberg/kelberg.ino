// #include <servomover.h>  /* Only for library detection */
// #include <Servo.h>  /* Only for library detection */
#include <Bounce2.h>

// Pins
// ====
//
// Red LEDs named after the signal/switch lever/etc they're for.
int LED_SWITCH_5 = 1;
int LED_SWITCH_1 = 1;
int LED_SIGNAL_A2 = 1;
int LED_SIGNAL_A1 = 1;
int LED_SWITCHING_PERMISSION = 1;
int LED_ROUTE_A = 1;
int LED_ROUTE_BERENBACH = 1;
// White LED
int LED_ROUTE_A_FIXED = 1;
// Levers and lock
int LEVER_SWITCH_5 = 1;
int LEVER_SWITCH_1 = 1;
int LEVER_SIGNAL_A2 = 1;
int LEVER_SIGNAL_A1 = 1;
int LOCK_SWITCHING_PERMISSION = 1;
// Route levers
int LEVER_ROUTE_A1 = 1;
int LEVER_ROUTE_A2 = 1;
int LEVER_ROUTE_BERENBACH1 = 1;
int LEVER_ROUTE_BERENBACH2 = 1;
// Push button for route fixing
int BUTTON_ROUTE_A_FIXED = 1;
// Output pins (pull to 0 means go out of the ground position)
int OUTPUT_SWITCH_5 = 1;
int OUTPUT_SWITCH_1 = 1;
int OUTPUT_SIGNAL_A1 = 1;
int OUTPUT_SIGNAL_A2 = 1;

// Array index numbers. These are columns in the locking table.
int SWITCH_5 = 0;
int SWITCH_1 = 1;
int SWITCHING_PERMISSION = 2;  // Note: PLUS means 'key in lock'.
int ROUTE_A1 = 3;
int ROUTE_A2 = 4;
int ROUTE_BERENBACH1 = 5;
int ROUTE_BERENBACH2 = 6;
int ROUTE_A_FIXED = 7;
int SIGNAL_A1 = 8;
int SIGNAL_A2 = 9;

// The two possible positions. Plus is the default, minus the actuated state.
bool PLUS = true;
bool MINUS = false;

// The states. All arrays with length 10.
const int ARRAY_SIZE = 10;
boolean current_position [ARRAY_SIZE] = { PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS, PLUS };
boolean blocked [ARRAY_SIZE] = { false, false, false, false, false, false, false, false, false, false };
boolean requirements_fulfilled [ARRAY_SIZE] = { true, true, true, false, false, false, false, false, false, false };
boolean position_is_ok [ARRAY_SIZE] = { true, true, true, true, true, true, true, true, true, true };


boolean movement_allowed (int number) {
  return (blocked[number] == false && requirements_fulfilled[number] == true);
}


void update_requirements() {
  // Requirements flow "upwards". The correct switch positions can enable
  // routes. Enabled routes can enable the route fixation button. Enabled
  // routes plus an enabled route fixation can enable signals.

  // Requirements fulfilled by the switches.
  requirements_fulfilled[ROUTE_A1] = false;
  requirements_fulfilled[ROUTE_A2] = false;
  requirements_fulfilled[ROUTE_BERENBACH1] = false;
  requirements_fulfilled[ROUTE_BERENBACH2] = false;
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
    requirements_fulfilled[ROUTE_BERENBACH1] = true;
  }
  if (current_position[SWITCH_1] == MINUS &&
      current_position[SWITCHING_PERMISSION] == PLUS) {
    requirements_fulfilled[ROUTE_BERENBACH2] = true;
  }

  // Requirements fulfilled by the route levers and/or the route fixer.
  requirements_fulfilled[ROUTE_A_FIXED] = false;
  requirements_fulfilled[SIGNAL_A1] = false;
  requirements_fulfilled[SIGNAL_A2] = false;
  if (current_position[ROUTE_A1] == MINUS ||
      current_position[ROUTE_A2] == MINUS) {
    requirements_fulfilled[ROUTE_A_FIXED] = true;
  }
  if (current_position[ROUTE_A2] == MINUS &&
      current_position[ROUTE_A_FIXED] == MINUS) {
    requirements_fulfilled[SIGNAL_A2] = true;
  }
  if (current_position[ROUTE_A1] == MINUS &&
      current_position[ROUTE_A_FIXED] == MINUS) {
    requirements_fulfilled[SIGNAL_A1] = true;
  }

}


void update_blocks() {
  // Blocks flow "downwards". A route lever blocks switches. A route fixation
  // blocks a route lever.

  // Reset all blocks.
  for (int i = 0; i < ARRAY_SIZE; i++) {
    blocked[i] = false;
  }

  // Route levers block each other. And they block switches.
  if (current_position[ROUTE_A1] == MINUS) {
    blocked[ROUTE_A2] = true;
    blocked[ROUTE_BERENBACH1] = true;
    blocked[ROUTE_BERENBACH2] = true;

    blocked[SWITCH_5] = true;
    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }
  if (current_position[ROUTE_A2] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_BERENBACH1] = true;
    blocked[ROUTE_BERENBACH2] = true;

    blocked[SWITCH_5] = true;
    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }
  if (current_position[ROUTE_BERENBACH1] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_A2] = true;
    blocked[ROUTE_BERENBACH2] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }
  if (current_position[ROUTE_BERENBACH2] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_A2] = true;
    blocked[ROUTE_BERENBACH1] = true;

    blocked[SWITCH_1] = true;
    blocked[SWITCHING_PERMISSION] = true;
  }

  // Route fixation blocks routes.
  if (current_position[ROUTE_A_FIXED] == MINUS) {
    blocked[ROUTE_A1] = true;
    blocked[ROUTE_A2] = true;
    // Special case: also keep yourself blocked (it is only un-blocked by
    // moving a signal back to PLUS).
    blocked[ROUTE_A_FIXED] = true;
  }
}


void change_position(int number,
                     int new_position) {
  current_position[number] = new_position;
  position_is_ok[number] = true;
  update_requirements();
  update_blocks();

  // Special case: signals going back to PLUS free the route fixation.
  if (number == SIGNAL_A1 || number == SIGNAL_A2) {
    if (new_position == PLUS) {
      change_position(ROUTE_A_FIXED, PLUS);
    }
  }
}


void react_to_movement(int number,
                       int new_position) {
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
  // TODO: attach the pins and hook up react_to_movement() functions.
}


void loop() {
  // TODO: update the LEDs and update the outgoing pins.
}
