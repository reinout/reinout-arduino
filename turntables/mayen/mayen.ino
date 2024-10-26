/*
 *   Basic example code for controlling a stepper with the AccelStepper library
 *
 *   by Dejan, https://howtomechatronics.com
 */

#include <AccelStepper.h>
#include <Bounce2.h>

// Pins
int ENABLE_PIN = 3;
int DIR_PIN = 4;
int STEP_PIN = 5;
int END_STOP_PIN = 7;

// Handy constants
long PHYSICAL_STEPS_PER_ROTATION = 200;
long MICROSTEPPING_FACTOR = 2;
long STEPS_PER_ROTATION = PHYSICAL_STEPS_PER_ROTATION * MICROSTEPPING_FACTOR;

// Buttons, stepper motors
AccelStepper motor(1, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Bounce end_stop = Bounce();

// States
boolean am_homing = true;


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

void configure_zero_point() {
  motor.setCurrentPosition(0);
  am_homing = false;
  configure_motor_for_operation();
  motor.moveTo(-10);
}

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  end_stop.attach(END_STOP_PIN, INPUT_PULLUP);
  end_stop.interval(1);
  configure_motor_for_homing();
  motor.moveTo(STEPS_PER_ROTATION * 1);
  enable_motor();
}

void loop() {
  end_stop.update();
  if (am_homing and end_stop.fell())
    {
      configure_zero_point();
      // Temp hack
      motor.moveTo(STEPS_PER_ROTATION * -10);
    }

  if (motor.distanceToGo() == 0)
    {
      disable_motor();
    }
  motor.run();
}
