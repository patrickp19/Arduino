#include <Servo.h>
//#include <SoftwareSerial.h>
//
//#define TxD 2
//#define RxD 3

//SoftwareSerial mySerial(RxD,TxD);
Servo myservo;
int angle = 0;

void setup() {
  // put your setup code here, to run once:
  //mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println(">> START<<");  
  myservo.attach(30);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(27, OUTPUT);
  

}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);  
  if(Serial.available() > 0)
  {
    char data;
    data = Serial.read();
    Serial.write(Serial.read());
    Serial.print(data);

    switch (data)
    {
      
      case '1': //FORWARD
          angle++;
          myservo.write(angle);
          break;
        
      case '2': //REVERSE
          myservo.write(angle);
          break;
        
      case '3': //FORWARD LEFT
          myservo.write(angle);
          break;
        
      case '4': //FORWARD RIGHT
          myservo.write(angle);
        break;
                 
      default: //If bluetooth module receives any value not listed above, both motors turn off
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(1000);  
    }
  }
}
