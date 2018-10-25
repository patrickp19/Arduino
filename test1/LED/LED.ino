#include <Servo.h>
Servo myservo;
int angle = 0;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(27, OUTPUT);
  myservo.attach(30);
  Serial.begin(9600);
}
 
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000); 
   
}
