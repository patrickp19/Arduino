#include <Servo.h>
Servo myservo;
int angle = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(">> START<<");  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(27, OUTPUT);
}

void loop() {
  if(Serial.available() > 0)
  {
    char data;
    data = Serial.read();
    Serial.write(Serial.read());

    switch (data)
    {
      
      case '1': //FORWARD
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
          angle++;
          myservo.write(angle);
        break;
        
      case '2': //REVERSE
          angle++;
          myservo.write(angle);
        break;
        
      case '3': //FORWARD LEFT
          angle++;
          myservo.write(angle);
        break;
        
      case '4': //FORWARD RIGHT
          angle++;
          myservo.write(angle);
        break;
               
      default: //If bluetooth module receives any value not listed above, both motors turn off
          break;
    }
  }
}
