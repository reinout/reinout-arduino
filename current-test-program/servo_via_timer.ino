#include <Servo.h>
#include <servomover.h>

LinearServoMover upper_arm = LinearServoMover(11, 1300, 1750, 1200);
LinearServoMover lower_arm = LinearServoMover(12, 1150, 1750, 1200);

int INITIAL_DELAY_MS = 2000;


void setup() {
  upper_arm.init();
  lower_arm.init();
  delay(INITIAL_DELAY_MS);
  upper_arm.move_to_max();
  lower_arm.move_to_max();

}


void loop() {
  upper_arm.perhaps_update();
  lower_arm.perhaps_update();
}
