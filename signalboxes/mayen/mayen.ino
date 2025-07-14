#include <Bounce2.h>
#include "Waveshare_LCD1602.h"


Waveshare_LCD1602 display(16,2);  // 16 characters, 2 lines.


void setup() {
  display.init();
  display.setCursor(0,0);
  // display.send_string("in - 3    14 - 4");
  display.send_string("in<--3    14 - 4");
  display.setCursor(0,1);
  // display.send_string("in<==3    14 - 4");
  // display.send_string("[ ] vastleggen  ");
  //display.send_string("[ ] vast [ ] sei");
  display.send_string("Fahrstrasse N3-");
}


void loop() {

}
