#include <Bounce2.h>

int RELAIS = 4;
int IR_SENSOR_PIN = 5;

Bounce ir_sensor = Bounce();


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(RELAIS, HIGH);
    pinMode(RELAIS, OUTPUT);
    ir_sensor.attach(IR_SENSOR_PIN, INPUT_PULLUP);
    ir_sensor.interval(50);
}

void loop() {
    ir_sensor.update();
    if (ir_sensor.fell())  // fell: iets gedetecteerd, naar 0.
    {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(RELAIS, LOW);
    // low means 'on'
    }

    if (ir_sensor.rose())
    {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAIS, HIGH);
    }
}
