#include <Servo.h>
Servo myservo;
int angle = 0;
void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(27, OUTPUT);
  myservo.attach(30);
  Serial.begin(9600);
}
 
void loop() {
  
  for (angle = 0; angle <= 180; angle ++){
    myservo.write(angle);
    delay(15);
  }
  for(angle = 180; angle > 0; angle--)
  {
    myservo.write(angle);
    delay(15);
  }


}
