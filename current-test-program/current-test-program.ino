#include <Bounce2.h>
#include "./detector.h"

int RELAIS = 4;
int IR_SENSOR_PIN_TUNNEL = 5;
int IR_SENSOR_PIN_CLOSE = 6;
int PUSH_BUTTON_PIN = 7;

Detector ir_sensor_tunnel = Detector(IR_SENSOR_PIN_TUNNEL);
Bounce ir_sensor_close = Bounce();
Bounce push_button = Bounce();


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(RELAIS, HIGH);
    pinMode(RELAIS, OUTPUT);
    ir_sensor_close.attach(IR_SENSOR_PIN_CLOSE, INPUT_PULLUP);
    ir_sensor_close.interval(50);
    push_button.attach(PUSH_BUTTON_PIN, INPUT_PULLUP);
    push_button.interval(50);
    ir_sensor_tunnel.init();
}

void loop() {
    ir_sensor_tunnel.update();
    ir_sensor_close.update();
    push_button.update();
    if (push_button.fell())  // fell: iets gedetecteerd, naar 0.
    {
    digitalWrite(RELAIS, LOW);
    // low means 'on'
    }

    if (ir_sensor_close.fell())
    {
    digitalWrite(RELAIS, HIGH);
    }

    if (ir_sensor_tunnel.active){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
}
