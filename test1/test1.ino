#include <BLEAdvertising.h>
#include <bluefruit.h>
#include <Servo.h>
Servo myservo;
int angle = 0;

void setup()
{
  // Other startup code here 
  // ...
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
  pinMode(27, OUTPUT);
    Serial.println("Bluefruit52 BLEUART Example");
  // Set up Advertising Packet
  setupAdv();
 
  // Start Advertising
  Bluefruit.Advertising.start();
}

void setupAdv(void)
{  
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  
  // There isn't enough room in the advertising packet for the
  // name so we'll place it on the secondary Scan Response packet
  Bluefruit.ScanResponse.addName();
}
 
void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
 
 
  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  if(Serial.available() > 0)
  {
    char data;
    data = Serial.read();
    Serial.write(Serial.read());

    switch (data)
    {
      
      case '1': //FORWARD
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
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
