/* Originally based om some AccelStepper example code
   by Dejan, https://howtomechatronics.com
 */

#include <AccelStepper.h>
#include <Bounce2.h>
#include <Keypad.h>

// Pins
int ENABLE_PIN = 15;
int DIR_PIN = 14;
int STEP_PIN = 2;
int END_STOP_PIN = 3;

// Handy constants
long PHYSICAL_STEPS_PER_ROTATION = 200;
long MICROSTEPPING_FACTOR = 2;
long STEPS_PER_ROTATION = PHYSICAL_STEPS_PER_ROTATION * MICROSTEPPING_FACTOR;

// Buttons, stepper motors
AccelStepper motor(1, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Bounce end_stop = Bounce();

// States
boolean am_homing = true;
boolean am_enabled = true;

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
  if (am_enabled == false) {
    am_enabled = true;
    digitalWrite(ENABLE_PIN, LOW);
    Serial.print("Enabled the motor.");
  }
}

void disable_motor() {
  if (am_enabled == true) {
    am_enabled = false;
    digitalWrite(ENABLE_PIN, HIGH);
    Serial.print("Disabled the motor. Current position: ");
    Serial.println(motor.currentPosition());
  }
}

void configure_motor_for_homing() {
  // Waarschijnlijk niet nodig, behalve als snelheid groter is dan normaal.
  // De .setCurrentPosition() zet de snelheid gelijk op nul.
  motor.setMaxSpeed(1000);
  motor.setAcceleration(99999);
}

void configure_motor_for_operation() {
  motor.setMaxSpeed(1000);
  motor.setAcceleration(400);
}

void configure_zero_point() {
  motor.setCurrentPosition(0);
  am_homing = false;
  Serial.println("End stop reached: configuring zero point.");
  configure_motor_for_operation();
  new_position(-10);
}

void new_position(long absolute) {
  enable_motor();
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" to ");
  Serial.println(absolute);
  motor.moveTo(absolute);
}

void new_relative_position(long relative) {
  enable_motor();
  Serial.print("Moving from ");
  Serial.print(motor.currentPosition());
  Serial.print(" with relative ");
  Serial.println(relative);
  motor.move(relative);
}

void handle_key(char key) {
  Serial.print("Handling key: ");
  Serial.println(key);
  if (key == '0') {
    new_position(0);
  }
  else if (key == '1') {
    new_position(-20);
  }
  else if (key == '2') {
    new_position(-0.5 * STEPS_PER_ROTATION);
  }
  else if (key == '3') {
    new_position(-1 * STEPS_PER_ROTATION);
  }
  else if (key == '5') {
    new_position(0.5 * STEPS_PER_ROTATION);
  }
  else if (key == '6') {
    new_position(1 * STEPS_PER_ROTATION);
  }
  else if (key == '8') {
    new_relative_position(4 * STEPS_PER_ROTATION);
  }
  else if (key == '9') {
    new_relative_position(-4 * STEPS_PER_ROTATION);
  }
}
// Idea: use * and # to move x revolutions forward/backward + print the current location?


void setup() {
  Serial.begin(9600);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  // TODO: setEnablePin()
  end_stop.attach(END_STOP_PIN, INPUT_PULLUP);
  end_stop.interval(1);
  enable_motor();
  configure_motor_for_homing();
  new_relative_position(-1 * STEPS_PER_ROTATION * 4);
}

void loop() {
  end_stop.update();
  char keypad_key = the_keypad.getKey();
  if (am_homing and end_stop.fell())
    {
      configure_zero_point();
    }

  if (not am_homing and keypad_key)
    {
      handle_key(keypad_key);
    }

  if (motor.distanceToGo() == 0 and am_enabled)
    {
      disable_motor();
    }
  motor.run();
}
