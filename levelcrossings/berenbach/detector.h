#ifndef detector_h
#define detector_h

#include <arduino.h>
#include <Bounce2.h>


class Detector
{
 public:
  Detector(int pin);
  void init();
  void update();
  bool active;

 private:
  int pin;
  bool deactivating;
  unsigned long deactivate_after;
  Bounce ir_sensor;
};

#endif
