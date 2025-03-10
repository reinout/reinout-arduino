/* Originally based om some AccelStepper example code
   by Dejan, https://howtomechatronics.com
 */

#include <AccelStepper.h>
#include <Bounce2.h>
#include <Keypad.h>

// Pins for steering the stepper motor driver.
int DIR_PIN = 5;
int STEP_PIN = 4;

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

// Buttons, stepper motors
AccelStepper motor(1, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Bounce begin_track = Bounce();
Bounce other_track = Bounce();
Bounce end_track = Bounce();

// States
int STATE_PRE_HOMING = 1;
int STATE_HOMING = 2;
int STATE_POST_HOMING = 3;g
int STATE_OPERATIONAL = 4;

int state;

// Positions
long POS0 = 1000;  // Starting position
long POS1 = 32150;
long POS2 = 29100;
long POS3 = 25100;
long POS4 = 21900;
long POS5 = 18789;
long POS6 = 16264;
long POS7 = 12820;
long POS8 = 10100;
long POS9 = 6020;
long POSa = 3220;
long POSb = 380;

// Keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad the_keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void new_position(long absolute) {
  long target = DIRECTION * absolute;
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" to ");
  Serial.println(target);
  motor.moveTo(target);
}

void new_relative_position(long relative) {
  long target = DIRECTION * relative;
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" with relative ");
  Serial.println(target);
  motor.move(target);
}

void handle_key(char key) {
  Serial.print("Handling key: ");
  Serial.println(key);
  if (key == '0') {
    new_position(POS0);
  }
  else if (key == '1') {
    new_position(POS1);
  }
  else if (key == '2') {
    new_position(POS2);
  }
  else if (key == '3') {
    new_position(POS3);
  }
  else if (key == '4') {
    new_position(POS4);
  }
  else if (key == '5') {
    new_position(POS5);
  }
  else if (key == '6') {
    new_position(POS6);
  }
  else if (key == '7') {
    new_position(POS7);
  }
  else if (key == '8') {
    new_position(POS8);
  }
  else if (key == '9') {
    new_position(POS9);
  }
  else if (key == 'A') {
    new_position(POSa);
  }
  else if (key == 'B') {
    new_position(POSb);
  }
  // Relative movement keys.
  else if (key == '*') {
    new_relative_position(-4000);
  }
  else if (key == '#') {
    new_relative_position(4000);
  }
  else if (key == 'C') {
    new_relative_position(-100);
  }
  else if (key == 'D') {
    new_relative_position(100);
  }
}

void start_pre_homing() {
  // Move a bit away from the begin track.
  state = STATE_PRE_HOMING;
  motor.setMaxSpeed(2000);
  new_relative_position(STEPS_PER_ROTATION * 1);
}

void start_homing() {
  // Move towards the begin track.
  state = STATE_HOMING;
  new_relative_position(STEPS_PER_ROTATION * -40);
}

void start_post_homing() {
  // Moved passed begin track, move SLOWLY back towards it until contact is made again.
  state = STATE_POST_HOMING;
  motor.setMaxSpeed(500);
  new_relative_position(STEPS_PER_ROTATION * 2);
}

void start_operation() {
  motor.setCurrentPosition(0);
  motor.moveTo(0);  // For completeness.
  state = STATE_OPERATIONAL;
  motor.setMaxSpeed(2000);
  motor.setAcceleration(4000);
}

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
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
  else if (state == STATE_HOMING and begin_track.rose()) {
    start_post_homing();
  }
  else if (state == STATE_POST_HOMING and begin_track.fell()) {
    start_operation();
  }
  else if (state == STATE_OPERATIONAL and keypad_key) {
    handle_key(keypad_key);
  }

  // Regular loop.
  motor.run();
}
