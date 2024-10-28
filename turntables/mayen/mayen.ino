/*
 *   Basic example code for controlling a stepper with the AccelStepper library
 *
 *   by Dejan, https://howtomechatronics.com
 */

#include <AccelStepper.h>
#include <Bounce2.h>
#include <Keypad.h>

// Pins
int ENABLE_PIN = 2;
int DIR_PIN = 3;
int STEP_PIN = 4;
int END_STOP_PIN = 5;

// Handy constants
long PHYSICAL_STEPS_PER_ROTATION = 200;
long MICROSTEPPING_FACTOR = 2;
long STEPS_PER_ROTATION = PHYSICAL_STEPS_PER_ROTATION * MICROSTEPPING_FACTOR;

// Buttons, stepper motors
AccelStepper motor(1, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Bounce end_stop = Bounce();

// States
boolean am_homing = true;

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
  digitalWrite(ENABLE_PIN, LOW);
}

void disable_motor() {
  digitalWrite(ENABLE_PIN, HIGH);
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
  configure_motor_for_operation();
  motor.moveTo(-10);
}

void handle_key(char key) {
  if (key == '1') {
    motor.moveTo(-20);
    enable_motor();
  }
  else if (key == '2') {
    motor.moveTo(-0.5 * STEPS_PER_ROTATION);
    enable_motor();
  }
  else if (key == '3') {
    motor.moveTo(-1 * STEPS_PER_ROTATION);
    enable_motor();
  }
}

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  end_stop.attach(END_STOP_PIN, INPUT_PULLUP);
  end_stop.interval(1);
  configure_motor_for_homing();
  motor.moveTo(STEPS_PER_ROTATION * 4);
  enable_motor();
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

  if (motor.distanceToGo() == 0)
    {
      disable_motor();
    }
  motor.run();
}
