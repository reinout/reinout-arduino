#include <Servo.h>

Servo main_signal;
Servo secondary_signal;

int MAIN_SIGNAL_MIN = 1300;
int MAIN_SIGNAL_MAX = 1750;
int MAIN_SIGNAL_DURATION_MS = 1200;
int main_signal_timestep_ms;
unsigned int main_signal_next_time_ms;
int main_signal_value;

int SECONDARY_SIGNAL_MIN = 1150;
int SECONDARY_SIGNAL_MAX = 1750;
int SECONDARY_SIGNAL_DURATION_MS = 1200;
int secondary_signal_timestep_ms;
unsigned int secondary_signal_next_time_ms;
int secondary_signal_value;

int INITIAL_DELAY_MS = 500;


void setup() {
  pinMode(13, OUTPUT);

  // main_signal.attach(11);
  // main_signal_timestep_ms = (MAIN_SIGNAL_DURATION_MS) / (MAIN_SIGNAL_MAX - MAIN_SIGNAL_MIN);
  // main_signal_value = MAIN_SIGNAL_MIN;
  // main_signal.writeMicroseconds(main_signal_value);
  // main_signal_next_time_ms = millis() + INITIAL_DELAY_MS;
  // main_signal.writeMicroseconds(main_signal_value);

  secondary_signal.attach(12);
  secondary_signal_timestep_ms = (SECONDARY_SIGNAL_DURATION_MS) / (SECONDARY_SIGNAL_MAX - SECONDARY_SIGNAL_MIN);
  secondary_signal_value = SECONDARY_SIGNAL_MIN;
  secondary_signal.writeMicroseconds(secondary_signal_value);
  secondary_signal_next_time_ms = millis() + INITIAL_DELAY_MS + 50;
  secondary_signal.writeMicroseconds(secondary_signal_value);

}


void perhaps_update_main_signal() {
  if (millis() > main_signal_next_time_ms) {
    main_signal_value += 1;
    if (main_signal_value > MAIN_SIGNAL_MAX) {
      main_signal.detach();
    }
    main_signal_next_time_ms += main_signal_timestep_ms;
    main_signal.writeMicroseconds(main_signal_value);
  }
}


void perhaps_update_secondary_signal() {
  if (millis() > secondary_signal_next_time_ms) {
    secondary_signal_value += 1;
    if (secondary_signal_value > SECONDARY_SIGNAL_MAX) {
      secondary_signal.detach();
    }
    secondary_signal_next_time_ms += secondary_signal_timestep_ms;
    secondary_signal.writeMicroseconds(secondary_signal_value);
  }
}


void loop() {
  // perhaps_update_main_signal();
  perhaps_update_secondary_signal();
}
