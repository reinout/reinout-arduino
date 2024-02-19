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
    Serial.begin(9600);
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

    if (push_button.fell()) {
      Serial.println("FELL");
    }
    if (push_button.rose()) {
      Serial.println("ROSE");
    }

    // Per state, look what transition is possible.
    if (state == STATE_REST){
      if (ir_sensor_b.active == true) {
        state = STATE_B1;
        Serial.println("State: b1");
      }
      if (push_button.fell()){
        // fell: iets gedetecteerd, naar 0.
        state = STATE_M1;
        Serial.println("State: m1, push button was pushed");
        // Direction Mayen, so we can deactivate quickly after passing
        // the level crossing.
        ir_sensor_a.enable_quick_deactivation();
      }
    }

    if (state == STATE_M1){
      if (ir_sensor_a.active == true) {
        state = STATE_M2;
        Serial.println("State: m2");
      }
    }

    if (state == STATE_M2){
      if (ir_sensor_b.active == true) {
        state = STATE_M3A;
        Serial.println("State: m3a");
      }
      if (ir_sensor_a.active == false) {
        state = STATE_M3B;
        Serial.println("State: m3b");
      }
    }

    if (state == STATE_M3A){
      if (ir_sensor_a.active == false) {
        state = STATE_M3C;
        Serial.println("State: m3a");
      }
    }

    if (state == STATE_M3B){
      if (ir_sensor_b.active == true) {
        state = STATE_M3C;
        Serial.println("State: m3c");
      }
    }

    if (state == STATE_M3C){
      if (ir_sensor_b.active == false) {
        state = STATE_REST;
        Serial.println("State: rest");
      }
    }

    if (state == STATE_B1){
      if (ir_sensor_a.active == true) {
        state = STATE_B2;
        Serial.println("State: b2");
      }
    }

    if (state == STATE_B2){
      if (ir_sensor_a.active == false) {
        state = STATE_REST;
        Serial.println("State: rest");
      }
    }

    // Handle the state.
    // Three state should have the relais high (=level crossing is off).
    if ((state == STATE_REST) || (state == STATE_M3B) || (state == STATE_M3C)) {
      digitalWrite(RELAIS, HIGH);
    }
    else {
      digitalWrite(RELAIS, LOW);
    }
    // Use the LED to signal that the state is in REST.
    if (state == STATE_REST) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
    }

}
