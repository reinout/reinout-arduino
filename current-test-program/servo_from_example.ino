/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int MIN = 1000;
int MAX = 2000;
int halfway;
int DURATION = 1200; // ms
int timestep;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
  myservo.attach(11);
  pinMode(13, OUTPUT);
}

void loop() {
  halfway = MAX - (MAX - MIN) / 2;
  timestep = DURATION / (MAX - MIN);
  Serial.println("Timestep = ");
  Serial.println(timestep);
  for (pos = MIN; pos <= halfway; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.writeMicroseconds(pos);              // tell servo to go to position in variable 'pos'
    delay(timestep);                       // waits 15ms for the servo to reach the position
  }
  delay(500);
  for (pos = halfway; pos <= MAX; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.writeMicroseconds(pos);              // tell servo to go to position in variable 'pos'
    delay(timestep);                       // waits 15ms for the servo to reach the position
  }
  delay(5000);
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  for (pos = MAX; pos >= MIN; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.writeMicroseconds(pos);              // tell servo to go to position in variable 'pos'
    delay(timestep);                       // waits 15ms for the servo to reach the position
  }
  delay(5000);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
}
