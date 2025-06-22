/* Originally based om some AccelStepper example code
   by Dejan, https://howtomechatronics.com
 */

#include <AccelStepper.h>
#include <Bounce2.h>
#include <Keypad.h>

// Pins for steering the stepper motor driver.
int DIR_PIN = 4;
int STEP_PIN = 3;

// Motor management
boolean motor_enabled = false;
int ENABLE_PIN = 2;

// Pins for reading info from the turntable tracks.
int BEGIN_TRACK_PIN = 16;

// Direction 'DIRECTION' is turning away counterclockwise from the begin track.
long DIRECTION = -1;  // 1 or -1, swap value depending on motor/turntable config.

// Handy constants
long PHYSICAL_STEPS_PER_ROTATION = 400;
long MICROSTEPPING_FACTOR = 16;
long STEPS_PER_ROTATION = PHYSICAL_STEPS_PER_ROTATION * MICROSTEPPING_FACTOR;
long SPEED = 4000;

// Buttons, stepper motors
AccelStepper motor(1, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Bounce begin_track = Bounce();

// States
int STATE_PRE_HOMING = 1;
int STATE_HOMING = 2;
int STATE_OPERATIONAL = 4;

int state;

// Positions
long POS0 = 0;
long POS_C9 = 6769;
long POS_C8 = 13127;
long POS_C7 = 19416;
long POS_C6 = 25842;
long POS_C5 = 31971;
long POS_C4 = 38460;
long POS_C3 = 44866;
long POS_C2 = 51211;
long POS_C1 = 57621;

long POS_B1 = 64145;
long POS_B2 = 57621;
long POS_B3 = 51211;
long POS_B4 = 44866;
long POS_B5 = 38460;
long POS_B6 = 31971;

long POS_A5 = 77555;
long POS_A4 = 84045;
long POS_A3 = 90355;
long POS_A2 = 100920;
long POS_A1 = 115810;

// "Bouncing" behaviour: move beyond position going towards begin track and then move
// back.
bool move_to_actual_target_after_stopping = false;
long actual_target = 0;

// Keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
char selected_letter = " ";
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad the_keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void enable_motor() {
  if (motor_enabled == false) {
    motor_enabled = true;
    digitalWrite(ENABLE_PIN, LOW);
    Serial.print("Enabled the motor.");
  }
}

void disable_motor() {
  if (motor_enabled == true) {
    motor_enabled = false;
    digitalWrite(ENABLE_PIN, HIGH);
    Serial.print("Disabled the motor. Current position: ");
    Serial.println(motor.currentPosition() * DIRECTION);
  }
}

void new_position(long target) {
  // 'target' is in absolute positions counterclockwise to zero.
  if ((motor.currentPosition() * DIRECTION > target) and (not target == 0)) {
    // Move a bit further and then go back.
    move_to_actual_target_after_stopping = true;
    actual_target = target;
    target = target - STEPS_PER_ROTATION / 4;
    Serial.print("Moving first to ");
    Serial.print(target);
    Serial.print(", then to the actual target ");
    Serial.println(actual_target);
  }
  else {
    move_to_actual_target_after_stopping = false;
  }
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" to ");
  Serial.println(target);
  enable_motor();
  motor.moveTo(target * DIRECTION);
}

void new_relative_position(long relative) {
  long target = DIRECTION * relative;
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" with relative ");
  Serial.println(target);
  enable_motor();
  motor.move(target);
}

void handle_key(char key) {
  Serial.print("Handling key: ");
  Serial.println(key);
  if (key == 'A' or key == 'B' or key == 'C' or key == 'D') {
    selected_letter = key;
  }

  if (selected_letter == 'A') {
    if (key == '5') {
      new_position(POS_A5);
    }
    else if (key == '4') {
      new_position(POS_A4);
    }
    else if (key == '3') {
      new_position(POS_A3);
    }
    else if (key == '2') {
      new_position(POS_A2);
    }
    else if (key == '1') {
      new_position(POS_A1);
    }
  }  // End of 'A'

  if (selected_letter == 'B') {
    if (key == '6') {
      new_position(POS_B6);
    }
    else if (key == '5') {
      new_position(POS_B5);
    }
    else if (key == '4') {
      new_position(POS_B4);
    }
    else if (key == '3') {
      new_position(POS_B3);
    }
    else if (key == '2') {
      new_position(POS_B2);
    }
    else if (key == '1') {
      new_position(POS_B1);
    }
  }  // End of 'B'

  if (selected_letter == 'C') {
    if (key == '0') {  // 10...
      new_position(POS0);
    }
    else if (key == '9') {
      new_position(POS_C9);
    }
    else if (key == '8') {
      new_position(POS_C8);
    }
    else if (key == '7') {
      new_position(POS_C7);
    }
    else if (key == '6') {
      new_position(POS_C6);
    }
    else if (key == '5') {
      new_position(POS_C5);
    }
    else if (key == '4') {
      new_position(POS_C4);
    }
    else if (key == '3') {
      new_position(POS_C3);
    }
    else if (key == '2') {
      new_position(POS_C2);
    }
    else if (key == '1') {
      new_position(POS_C1);
    }
  }  // End of 'C'

  if (selected_letter == 'D') {
    if (key == '1') {
      new_relative_position(10);
    }
    else if (key == '4') {
      new_relative_position(100);
    }
    else if (key == '7') {
      new_relative_position(1000);
    }
    else if (key == '3') {
      new_relative_position(-10);
    }
    else if (key == '6') {
      new_relative_position(-100);
    }
    else if (key == '9') {
      new_relative_position(-1000);
    }
  }  // End of 'D'

  // Relative movement keys.
  else if (key == '*') {
    new_relative_position(-100);
  }
  else if (key == '#') {
    new_relative_position(100);
  }
}

void start_pre_homing() {
  // Move a bit away from the begin track.
  state = STATE_PRE_HOMING;
  motor.setMaxSpeed(SPEED);
  new_relative_position(STEPS_PER_ROTATION * 1);
}

void start_homing() {
  // Move towards the begin track.
  state = STATE_HOMING;
  new_relative_position(STEPS_PER_ROTATION * -40);
}

void start_operation() {
  motor.setCurrentPosition(0);
  motor.moveTo(0);  // For completeness.
  state = STATE_OPERATIONAL;
  motor.setMaxSpeed(SPEED);
  motor.setAcceleration(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  begin_track.attach(BEGIN_TRACK_PIN, INPUT_PULLUP);
  begin_track.interval(1);
  motor.setAcceleration(99999);
  start_pre_homing();
}

void loop() {
  begin_track.update();
  char keypad_key = the_keypad.getKey();

  if (state == STATE_PRE_HOMING and motor.distanceToGo() == 0) {
    start_homing();
  }
  else if (state == STATE_HOMING and begin_track.fell()) {
    start_operation();
  }
  else if (state == STATE_OPERATIONAL and keypad_key) {
    handle_key(keypad_key);
  }

  if (motor_enabled and motor.distanceToGo() == 0) {
    if (move_to_actual_target_after_stopping == true) {
      new_position(actual_target);
    }
    else {
      disable_motor();
    }
  }

  // Regular loop.
  motor.run();
}
