#include <Servo.h>
#include <servomover.h>
#include <signalmover.h>
#include <Bounce2.h>

LinearServoMover upper_arm = LinearServoMover(4, 1201, 1850, 800);

SingleSignalMover signal = SingleSignalMover(&upper_arm, 2);


void setup() {
  signal.init();
}


void loop() {
  signal.perhaps_update();
}
