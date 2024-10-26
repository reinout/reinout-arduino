/*
 *   Basic example code for controlling a stepper with the AccelStepper library
 *
 *   by Dejan, https://howtomechatronics.com
 */

#include <AccelStepper.h>

#define ENABLE_PIN 3
#define DIR_PIN 4
#define STEP_PIN 5

#define STEPS_PER_ROTATION 200
#define MICROSTEPPING_FACTOR 2


// Define the stepper motor and the pins that is connected to
AccelStepper motor(1, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)


void enable_motor() {
  digitalWrite(ENABLE_PIN, LOW);
}

void disable_motor() {
  digitalWrite(ENABLE_PIN, HIGH);
}

void configure_motor_for_homing() {
  motor.setMaxSpeed(1000);
  motor.setAcceleration(99999);
}

void configure_motor_for_operation() {
  motor.setMaxSpeed(1000);
  motor.setAcceleration(400);
}

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  configure_motor_for_operation();
  motor.moveTo(STEPS_PER_ROTATION * MICROSTEPPING_FACTOR * 10);
  enable_motor();
}

void loop() {
  if (motor.distanceToGo() == 0)
    {
      disable_motor();
    }
  motor.run();
}
