#include <Bounce2.h>
#include "./detector.h"

int RELAIS = 4;
int IR_SENSOR_PIN_B = 5;
int IR_SENSOR_PIN_A = 6;
int PUSH_BUTTON_PIN = 7;

Detector ir_sensor_a = Detector(IR_SENSOR_PIN_A);
Detector ir_sensor_b = Detector(IR_SENSOR_PIN_B);
Bounce push_button = Bounce();

// States
int STATE_REST = 0;
int STATE_M1 = 10;
int STATE_M2 = 20;
int STATE_M3A = 30;
int STATE_M3B = 31;
int STATE_M3C = 32;
int STATE_B1 = 40;
int STATE_B2 = 41;

int state;


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(RELAIS, HIGH);
    pinMode(RELAIS, OUTPUT);
    push_button.attach(PUSH_BUTTON_PIN, INPUT_PULLUP);
    push_button.interval(50);
    ir_sensor_a.init();
    ir_sensor_b.init();
    state = STATE_REST;
}

void loop() {
    ir_sensor_a.update();
    ir_sensor_b.update();
    push_button.update();

    // Per state, look what transition is possible.
    if (state == STATE_REST){
      if (ir_sensor_b.active == true) {
        state = STATE_B1;
      }
      if (push_button.fell()){
        // fell: iets gedetecteerd, naar 0.
        state = STATE_M1;
      }
    }

    if (state == STATE_M1){
      if (ir_sensor_a.active == true) {
        state = STATE_M2;
      }
    }

    if (state == STATE_M2){
      if (ir_sensor_b.active == true) {
        state = STATE_M3A;
      }
      if (ir_sensor_a.active == false) {
        state = STATE_M3B;
      }
    }

    if (state == STATE_M3A){
      if (ir_sensor_a.active == false) {
        state = STATE_M3C;
      }
    }

    if (state == STATE_M3B){
      if (ir_sensor_b.active == true) {
        state = STATE_M3C;
      }
    }

    if (state == STATE_M3C){
      if (ir_sensor_b.active == false) {
        state = STATE_REST;
      }
    }

    if (state == STATE_B1){
      if (ir_sensor_a.active == true) {
        state = STATE_B2;
      }
    }

    if (state == STATE_B2){
      if (ir_sensor_a.active == false) {
        state = STATE_REST;
      }
    }

    // Handle the state.
    if ((state == STATE_REST) || (state == STATE_M3B) || (state == STATE_M3C)) {
      digitalWrite(RELAIS, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
    }
    else {
      digitalWrite(RELAIS, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
    }

}
