#include <Servo.h>
#include <servomover.h>
#include <signalmover.h>
#include <Bounce2.h>

int PIN_SWITCH_1_INPUT = A1;
int PIN_SWITCH_5_INPUT = A2;

int PIN_RELAIS_1 = 4;
int PIN_RELAIS_5 = 3;

int PIN_SERVO_1 = 6;
int PIN_SERVO_5 = 5;


LinearServoMover servo_1 = LinearServoMover(PIN_SERVO_1, 1000, 1850, 1000);
LinearServoMover servo_5 = LinearServoMover(PIN_SERVO_5, 1000, 1850, 1000);

SwitchMover switch_1 = SwitchMover(&servo_1, PIN_SWITCH_1_INPUT, PIN_RELAIS_1);
SwitchMover switch_5 = SwitchMover(&servo_5, PIN_SWITCH_5_INPUT, PIN_RELAIS_5);


void setup() {
  switch_1.init();
  switch_5.init();
}


void loop() {
  switch_1.perhaps_update();
  switch_5.perhaps_update();
}
