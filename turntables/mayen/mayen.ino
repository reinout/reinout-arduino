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
int ENABLE_PIN = 17;

// Pins for reading info from the turntable tracks.
int BEGIN_TRACK_PIN = 16;
int OTHER_TRACK_PIN = 15;
int END_TRACK_PIN = 14;

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
Bounce other_track = Bounce();
Bounce end_track = Bounce();

// States
int STATE_PRE_HOMING = 1;
int STATE_HOMING = 2;
int STATE_OPERATIONAL = 4;

int state;

// Positions
long POS0 = 0;
long POS1 = 6769;
long POS2 = 13127;
long POS3 = 19616;
long POS4 = 26042;
long POS5 = 32271;
long POS6 = 38760;
long POS7 = 45166;
long POS8 = 51511;
long POS9 = 58121;  // fuzzy
long POS10 = 64945;
long POS11 = 77455;
long POS12 = 83745;
long POS13 = 90355;
long POS14 = 100620;
long POS15 = 115810;

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
    Serial.println(motor.currentPosition());
  }
}

void new_position(long absolute) {
  long target = DIRECTION * absolute;
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" to ");
  Serial.println(target);
  enable_motor();
  motor.moveTo(target);
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
      new_position(POS11);
    }
    else if (key == '4') {
      new_position(POS12);
    }
    else if (key == '3') {
      new_position(POS13);
    }
    else if (key == '2') {
      new_position(POS14);
    }
    else if (key == '1') {
      new_position(POS15);
    }
  }  // End of 'A'

  if (selected_letter == 'B') {
    if (key == '6') {
      new_position(POS5);
    }
    else if (key == '5') {
      new_position(POS6);
    }
    else if (key == '4') {
      new_position(POS7);
    }
    else if (key == '3') {
      new_position(POS8);
    }
    else if (key == '2') {
      new_position(POS9);
    }
    else if (key == '1') {
      new_position(POS10);
    }
  }  // End of 'B'

  if (selected_letter == 'C') {
    if (key == '0') {  // 10...
      new_position(POS0);
    }
    else if (key == '9') {
      new_position(POS1);
    }
    else if (key == '8') {
      new_position(POS2);
    }
    else if (key == '7') {
      new_position(POS3);
    }
    else if (key == '6') {
      new_position(POS4);
    }
    else if (key == '5') {
      new_position(POS5);
    }
    else if (key == '4') {
      new_position(POS6);
    }
    else if (key == '3') {
      new_position(POS7);
    }
    else if (key == '2') {
      new_position(POS8);
    }
    else if (key == '1') {
      new_position(POS9);
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
  // motor.setAcceleration(99999);
}

void record_track() {
  Serial.print("Track number ");
  Serial.print(upcoming_track_number);
  Serial.print(" has position ");
  Serial.println(motor.currentPosition());
  upcoming_track_number += 1;
}

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  begin_track.attach(BEGIN_TRACK_PIN, INPUT_PULLUP);
  begin_track.interval(1);
  other_track.attach(OTHER_TRACK_PIN, INPUT_PULLUP);
  other_track.interval(1);
  end_track.attach(END_TRACK_PIN, INPUT_PULLUP);
  end_track.interval(1);
  motor.setAcceleration(99999);
  start_pre_homing();
}

void loop() {
  begin_track.update();
  other_track.update();
  end_track.update();
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
    disable_motor();
  }

  // Regular loop.
  motor.run();
}
