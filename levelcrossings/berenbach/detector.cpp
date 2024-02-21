#include <arduino.h>
#include <Bounce2.h>
#include "detector.h"

unsigned long DEFAULT_DEACTIVATION_TIMEOUT = 1000 * 5;
unsigned long QUICK_DEACTIVATION_TIMEOUT = 1000 * 1;
int DETECTOR_NOTHING = 0;
int DETECTOR_DETECTED_SOMETHING = 1;
int DETECTOR_DEACTIVATING = 2;
int DETECTOR_DEACTIVATED = 3;


Detector::Detector(int pin){
  Detector::pin = pin;
  Detector::active = false;
  Detector::deactivating = false;
  Detector::deactivate_after = millis();
  Detector::deactivation_timeout = DEFAULT_DEACTIVATION_TIMEOUT;
  Detector::ir_sensor = Bounce();
}

void Detector::init(){
  ir_sensor.attach(pin, INPUT_PULLUP);
  ir_sensor.interval(200);
}

void Detector::enable_quick_deactivation(){
  deactivation_timeout = QUICK_DEACTIVATION_TIMEOUT;
}

int Detector::update(){
  int result = DETECTOR_NOTHING;
  ir_sensor.update();
  if (ir_sensor.fell()) {
    // We detected something!
    active = true;
    deactivating = false;
    result = DETECTOR_DETECTED_SOMETHING;
  }
  if (ir_sensor.rose()) {
    // We're not detecting anything, but it might be just the space between two cars.
    deactivating = true;
    deactivate_after = millis() + deactivation_timeout;
    result = DETECTOR_DEACTIVATING;
  }
  if (deactivating && (millis() > deactivate_after)){
    // We're deactivating and haven't seen a thing for quite some time.
    active = false;
    deactivating = false;
    deactivation_timeout = DEFAULT_DEACTIVATION_TIMEOUT;
    result = DETECTOR_DEACTIVATED;
  }
  return result;
}
