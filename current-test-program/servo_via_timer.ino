#include <Servo.h>
#include <servomover.h>
#include <Bounce2.h>

LinearServoMover upper_arm = LinearServoMover(11, 1301, 1750, 1200);
LinearServoMover lower_arm = LinearServoMover(12, 1150, 1750, 1200);

Bounce hp1_button = Bounce();
Bounce hp2_button = Bounce();

int INITIAL_DELAY_MS = 2000;


void setup() {
  hp1_button.attach(2, INPUT_PULLUP);
  hp2_button.attach(3, INPUT_PULLUP);
  hp1_button.interval(20);
  hp2_button.interval(20);

  upper_arm.init();
  lower_arm.init();
}


void loop() {
  hp1_button.update();
  hp2_button.update();

  if (hp1_button.rose() ) {
    upper_arm.move_to_min();
    lower_arm.move_to_min();
  }
  if (hp1_button.fell() ) {
    upper_arm.move_to_max();
    lower_arm.move_to_min();
  }

  if (hp2_button.rose() ) {
    upper_arm.move_to_min();
    lower_arm.move_to_min();
  }
  if (hp2_button.fell() ) {
    upper_arm.move_to_max();
    lower_arm.move_to_max();
  }

  upper_arm.perhaps_update();
  lower_arm.perhaps_update();
}
