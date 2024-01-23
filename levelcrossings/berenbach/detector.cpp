#include <arduino.h>
#include <Bounce2.h>
#include "detector.h"

unsigned long DEACTIVATE_TIMEOUT = 1000 * 5;


Detector::Detector(int pin){
  Detector::pin = pin;
  Detector::active = false;
  Detector::deactivating = false;
  Detector::deactivate_after = millis();
  Detector::ir_sensor = Bounce();
}

void Detector::init(){
  ir_sensor.attach(pin, INPUT_PULLUP);
  ir_sensor.interval(50);
}

void Detector::update(){
  ir_sensor.update();
  if (ir_sensor.fell()) {
    // We detected something!
    active = true;
    deactivating = false;
  }
  if (ir_sensor.rose()) {
    // We're not detecting anything, but it might be just the space between two cars.
    deactivating = true;
    deactivate_after = millis() + DEACTIVATE_TIMEOUT;
  }
  if (deactivating && (millis() > deactivate_after)){
    // We're deactivating and haven't seen a thing for quite some time.
    active = false;
    deactivating = false;
  }
}

// class Detector
// {
//  public:
//   Detector(int pin);
//   void init();
//   void update();
//   bool active;

//  private:
//   int pin;
//   bool deactivating;
//   unsigned long deactivate_after;
//   Bounce ir_sensor;
// };
