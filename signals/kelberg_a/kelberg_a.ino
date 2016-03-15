#include <servomover.h>  /* Only for library detection */
#include <Servo.h>  /* Only for library detection */
#include <pulsemover.h>
#include <Bounce2.h>

int HP1_BUTTON_PIN = 2;
int HP2_BUTTON_PIN = 3;
int MAIN_BASE = 7;
int MAIN_ACTUATED = 8;
int SECONDARY_BASE = 6;
int SECONDARY_ACTUATED = 5;

PulseMover upper_arm = PulseMover(MAIN_BASE, MAIN_ACTUATED);
PulseMover lower_arm = PulseMover(SECONDARY_BASE, SECONDARY_ACTUATED);
Bounce hp1_button = Bounce();
Bounce hp2_button = Bounce();


void setup() {
  hp1_button.attach(HP1_BUTTON_PIN, INPUT_PULLUP);
  hp1_button.interval(20);
  hp2_button.attach(HP2_BUTTON_PIN, INPUT_PULLUP);
  hp2_button.interval(20);
  upper_arm.init();
  lower_arm.init();
}


void loop() {
  hp1_button.update();
  if (hp1_button.fell() ) {
    /* fell = pulled to ground, which means activated */
    upper_arm.move_to_actuated();
    lower_arm.move_to_base();
  }
  if (hp1_button.rose() ) {
    /* Back to default */
    upper_arm.move_to_base();
    lower_arm.move_to_base();
  }

  hp2_button.update();
  if (hp2_button.fell() ) {
    /* fell = pulled to ground, which means activated */
    upper_arm.move_to_actuated();
    lower_arm.move_to_actuated();
  }
  if (hp2_button.rose() ) {
    /* Back to default */
    upper_arm.move_to_base();
    lower_arm.move_to_base();
  }

  upper_arm.perhaps_update();
  lower_arm.perhaps_update();
}
