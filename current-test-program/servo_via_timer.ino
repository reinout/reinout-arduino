#include <Servo.h>

Servo servo1;

int SERVO1_MIN = 1250;
int SERVO1_MAX = 1750;
unsigned long SERVO1_DURATION_MS = 1200;
unsigned long servo1_timestep_micros;
unsigned long servo1_next_time_micros;
int servo1_value;
unsigned long INITIAL_DELAY_MS = 500;


void setup() {
  servo1.attach(11);
  pinMode(13, OUTPUT);
  servo1_timestep_micros = (SERVO1_DURATION_MS * 1000) / (SERVO1_MAX - SERVO1_MIN);
  servo1_value = SERVO1_MIN;
  servo1.writeMicroseconds(servo1_value);
  servo1_next_time_micros = micros() + 1000 * INITIAL_DELAY_MS;
}


void perhaps_update_servo1() {
  if (micros() > servo1_next_time_micros) {
    servo1_value += 1;
    if (servo1_value > SERVO1_MAX) {
      // delay(1000);
      // servo1_value = SERVO1_MIN;
      // servo1_next_time_micros = micros();
      servo1.detach();
    }
    servo1_next_time_micros += servo1_timestep_micros;
    servo1.writeMicroseconds(servo1_value);
  }
}


void loop() {
  perhaps_update_servo1();
}