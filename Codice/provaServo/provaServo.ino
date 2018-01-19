#include <Servo.h>
int servoPin = 7;
Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  servo.write(-10);
  delay(10);
}
