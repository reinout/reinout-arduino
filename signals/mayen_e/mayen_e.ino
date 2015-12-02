#include <Servo.h>
#include <servomover.h>
#include <signalmover.h>
#include <Bounce2.h>

LinearServoMover upper_arm = LinearServoMover(4, 1201, 1850, 800);
int HP1_BUTTON_PIN = 2;

SingleSignalMover signal = SingleSignalMover(&upper_arm, HP1_BUTTON_PIN);


void setup() {
  signal.init();
}


void loop() {
  signal.perhaps_update();
}
