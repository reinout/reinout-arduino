/* Originally based om some AccelStepper example code
   by Dejan, https://howtomechatronics.com
 */

#include <AccelStepper.h>
#include <Bounce2.h>
#include <Keypad.h>

// Pins
int SLEEP_PIN = 16;  // ENABLE
int DIR_PIN = 14;  // TODO
int STEP_PIN = 15;  // TODO
int END_STOP_PIN = 3;

// Direction 'DIRECTION' is turning away from the zero point.
long DIRECTION = -1;  // 1 or -1, swap value depending on motor/turntable config.

// Handy constants
long PHYSICAL_STEPS_PER_ROTATION = 200;
long MICROSTEPPING_FACTOR = 8;
long STEPS_PER_ROTATION = PHYSICAL_STEPS_PER_ROTATION * MICROSTEPPING_FACTOR;

// Buttons, stepper motors
AccelStepper motor(1, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Bounce end_stop = Bounce();

// Motor management
boolean motor_enabled = false;

// States
int STATE_PRE_HOMING = 1;
int STATE_HOMING = 2;
int STATE_POST_HOMING = 3;
int STATE_OPERATIONAL = 4;

int state;

// Positions
long POS0 = 1000;  // Starting position
long POS1 = 1000;
long POS2 = 1000;
long POS3 = 1000;
long POS4 = 1000;
long POS5 = 2800;
long POS6 = 2400;
long POS7 = 4100;
long POS8 = 2950;
long POS9 = 1850;
long POSa = 1020;
long POSb = 20;

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


void enable_motor() {
  if (motor_enabled == false) {
    motor_enabled = true;
    digitalWrite(SLEEP_PIN, HIGH);
    Serial.print("Enabled the motor.");
  }
}

void disable_motor() {
  if (motor_enabled == true) {
    motor_enabled = false;
    digitalWrite(SLEEP_PIN, LOW);
    Serial.print("Disabled the motor. Current position: ");
    Serial.println(motor.currentPosition());
  }
}

void new_position(long absolute) {
  long target = DIRECTION * absolute;
  enable_motor();
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" to ");
  Serial.println(target);
  motor.moveTo(target);
}

void new_relative_position(long relative) {
  long target = DIRECTION * relative;
  enable_motor();
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
  else if (key == '*') {
    new_relative_position(-1000);
  }
  else if (key == '#') {
    new_relative_position(1000);
  }
}

void start_pre_homing() {
  // Move a bit away from the end stop.
  state = STATE_PRE_HOMING;
  motor.setMaxSpeed(1000);
  new_relative_position(STEPS_PER_ROTATION * 1);
}

void start_homing() {
  // Move towards the end stop
  state = STATE_HOMING;
  new_relative_position(STEPS_PER_ROTATION * -12);
}

void start_post_homing() {
  // End stop reached, move SLOWLY away from it.
  state = STATE_POST_HOMING;
  motor.setMaxSpeed(50);
  new_relative_position(STEPS_PER_ROTATION * 1);
}

void start_operation() {
  motor.setCurrentPosition(0);
  motor.moveTo(0);  // For completeness.
  state = STATE_OPERATIONAL;
  motor.setMaxSpeed(1000);
  motor.setAcceleration(400);
  new_position(POS0);
}


void setup() {
  Serial.begin(9600);
  pinMode(SLEEP_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  // TODO: setEnablePin()
  end_stop.attach(END_STOP_PIN, INPUT_PULLUP);
  end_stop.interval(1);
  enable_motor();
  motor.setAcceleration(99999);
  start_pre_homing();
}

void loop() {
  end_stop.update();
  char keypad_key = the_keypad.getKey();

  if (state == STATE_PRE_HOMING and motor.distanceToGo() == 0) {
    start_homing();
  }
  else if (state == STATE_HOMING and end_stop.fell()) {
    start_post_homing();
  }
  else if (state == STATE_POST_HOMING and end_stop.rose()) {
    start_operation();
  }
  else if (state == STATE_OPERATIONAL and keypad_key) {
    handle_key(keypad_key);
  }

  if (motor_enabled and motor.distanceToGo() == 0) {
    disable_motor();
  }

  // Safety valve
  if (state == STATE_OPERATIONAL and end_stop.fell()) {
    start_post_homing();
  }

  // Regular loop.
  motor.run();
}
