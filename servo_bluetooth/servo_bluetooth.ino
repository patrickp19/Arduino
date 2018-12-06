#include <Wire.h>

/*
   NOTES: SG90 Servos operate in PWM microseconds from 500 - 2500 for 180 DOF
   writeMicroseconds does not like calculations in parameter
   Written by Ryan Miller and Patrick Phattharaampornchai
*/
#include <Servo.h>
#include <SoftwareSerial.h>
#include <SimpleKalmanFilter.h>

#define TxD 31 // BT COM TX
#define RxD 11 // BT COM RX
#define DRIVE_PIN 30
#define STEER_PIN 15

#define DEBUG 0

SoftwareSerial BTSerial(RxD, TxD);
Servo drive, steer;
int spd = 0;
int angle = 0;


#define MPU 0x68
#define g 9.81 // m/s^2
#define acc 16384 // for 2g setting on accelerometer, 1g = 16384

//int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
//int16_t estAX, estAY, estAZ, estTemp, estGX, estGY, estGZ;
// accelerometer default is +/- 2g sensitivity
// gyro default is 250 deg/sec sensitivity

//SimpleKalmanFilter AX(100, 50, 0.1),
//                   AY(100, 100, 0.1),
//                   AZ(100, 100, 0.1),
//                   Temp(1, 1, 0.01),
//                   GX(100, 100, 0.1),
//                   GY(100, 100, 0.1),
//                   GZ(100, 100, 0.1);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write((byte) 0x00);
  Wire.endTransmission(true);

  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println(">> START<<");
  steer.attach(STEER_PIN);
  drive.attach(DRIVE_PIN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(27, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  //readSensors();
  String data = readBT();
  if (data.length() == 0)
    return;
  BTSerial.print("ABC");

  //Serial.println(data + "  !!!!");
  //BTSerial.println("LOOK,");

  char motor = data[0];
  data[0] = ' ';
  int value = data.toInt() * 5;
  if (DEBUG) {
    if (motor == '#')
      Serial.print("Drive -> ");
    else if ( motor == '$')
      Serial.print("Steer -> ");
    Serial.print(data + " -> "); //prints string to serial port out
    Serial.println(value);
  }
  if (motor == '#') {
    if (value > -100 and value < 100) {
      spd = value * 7 + 1500;
      drive.writeMicroseconds(spd);
    }
  }
  else if (motor == '$') {
    if (value > -100 and value < 100) {
      angle = value * 10 + 1500;
      steer.writeMicroseconds(angle);
    }
  }

  //    //BTSerial.write(Serial.read());
  //    switch (data)
  //    {
  //
  //      case '1': //FORWARD
  //          if(spd < 10)
  //            spd++;
  //          drive.writeMicroseconds(1500 + spd*100);
  //          Serial.println("F");
  //          break;
  //
  //      case '2': //REVERSE
  //          if(spd > -10)
  //            spd--;
  //          drive.writeMicroseconds(1500 + spd*100);
  //          Serial.println("R");
  //          break;
  //
  //      case '3': //FORWARD LEFT
  //          if(angle < 10)
  //            angle++;
  //          steer.writeMicroseconds(1500 + angle*100);
  //          Serial.println("FL");
  //          break;
  //
  //      case '4': //FORWARD RIGHT
  //          if(angle > -10)
  //            angle--;
  //          steer.writeMicroseconds(1500 + angle*100);
  //          Serial.println("FR");
  //          break;
  //
  //      default: //If bluetooth module receives any value not listed above, both motors turn off
  //          break;
  //    }
}

// works for 1-character reading
char readBT1C()
{
  char data = '0';
  if (BTSerial.available())
  {
    data = BTSerial.read();
    delay(100);
    while (BTSerial.available())
      BTSerial.read();
  }
  return data;
}

/*
   Code taken from https://forum.arduino.cc/index.php?topic=93867.0
   comment by elan_arr
*/
String readBT()
{
  String readStr;
  //expect a string like wer,qwe rty,123 456,hyre kjhg,
  //or like hello world,who are you?,bye!,
  while (BTSerial.available()) {
    delay(10);  //small delay to allow input buffer to fill

    char c = BTSerial.read();  //gets one byte from serial buffer
    if (c == ',') {
      break;
    }  //breaks out of capture loop to print readstring
    readStr += c;
  } //makes the string readString
  return readStr;
}

//void readSensors() {
//  Wire.beginTransmission(MPU);
//  Wire.write(0x3B);
//  Wire.endTransmission(false);
//  Wire.requestFrom(MPU, 14, true);
//  AcX = Wire.read() << 8 | Wire.read();
//  AcY = Wire.read() << 8 | Wire.read();
//  AcZ = Wire.read() << 8 | Wire.read();
//  Tmp = Wire.read() << 8 | Wire.read();
//  GyX = Wire.read() << 8 | Wire.read();
//  GyY = Wire.read() << 8 | Wire.read();
//  GyZ = Wire.read() << 8 | Wire.read();
//
//  estAX = AX.updateEstimate(AcX) - 900;
//  estAY = AY.updateEstimate(AcY) - 500;
//  estAZ = AZ.updateEstimate(AcZ) + 1250;
//  estTemp = Temp.updateEstimate(Tmp);
//  estGX = GX.updateEstimate(GyX) + 200;
//  estGY = GY.updateEstimate(GyY) + 80;
//  estGZ = GZ.updateEstimate(GyZ) + 40;
//}
