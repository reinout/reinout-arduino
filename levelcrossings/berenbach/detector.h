#ifndef detector_h
#define detector_h

#include <arduino.h>
#include <Bounce2.h>


class Detector
{
 public:
  Detector(int pin);
  void init();
  int update();
  void enable_quick_deactivation();
  bool active;

 private:
  int pin;
  bool deactivating;
  unsigned long deactivate_after;
  unsigned long deactivation_timeout;
  Bounce ir_sensor;
};

#endif
