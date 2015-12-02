#include <Servo.h>
#include <servomover.h>
#include <signalmover.h>
#include <Bounce2.h>

LinearServoMover upper_arm = LinearServoMover(11, 1301, 1750, 1200);
LinearServoMover lower_arm = LinearServoMover(12, 1150, 1750, 1200);
int HP1_BUTTON_PIN = 2
int HP2_BUTTON_PIN = 3

DualSignalMover signal = DualSignalMover(&upper_arm, HP1_BUTTON_PIN,
                                         &lower_arm, HP2_BUTTON_PIN);


void setup() {
  signal.init();
}


void loop() {
  signal.perhaps_update();
}
