/** 
 * @author Ryan Miller, Patrick Phattharaampornchai  
 * @date 2018 
 * @description EGEN 310 Arduino Code for the Bluetooth-controlled RC car 
 */ 
#include <Wire.h> 
#include <Servo.h> 
#include <SoftwareSerial.h> 
/* 
 * NOTES: SG90 Servos operate in PWM microseconds from 500 - 2500 for 180 DOF 
*/ 
#define TxD 31          // BT COM TX 
#define RxD 11           // BT COM RX 
#define DRIVE_PIN 30    // Drive Servo PWM Out 
#define STEER_PIN 15    // Steering Servo PWM Out 
#define CURRENT_PIN A3  // Analog current sense pin input 
#define VOLTAGE_PIN A4  // Analog voltage sense pin input 
#define SENSITIVITY 0.5 // noise filtering parameter for voltage and current sensing 
#define SEND_DELTA 5000 // time delay for sending data back to control app 
#define DEBUG 0         // change to 1 to enter debug mode 
/* Define and initialize global variables */ 
SoftwareSerial BTSerial(RxD, TxD); 
Servo drive, steer; 
int spd = 0; 
int angle = 0; 
  
int currentRaw = 0, voltageRaw = 0; 
float voltage = 0, current = 0; 
float power = 0; 
double energy = 0; 
  
long lastDataTX = 0; 
long t0 = 0; 
long t = 0; 
  
void setup() { 
  // run once on startup 
  Serial.begin(9600);   // set up Serial COMs for debugging 
  BTSerial.begin(9600); // set up Bluetooth COMs for HC05 communication 
  // connect and set up pins 
  steer.attach(STEER_PIN); 
  drive.attach(DRIVE_PIN); 
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(27, OUTPUT); 
  pinMode(CURRENT_PIN, INPUT); 
  pinMode(VOLTAGE_PIN, INPUT); 
  
  digitalWrite(LED_BUILTIN, LOW); 
  // initialize time-tracking variables 
  lastDataTX = millis(); 
  t0 = millis(); 
} 
void loop() 
{ 
  long strt = millis(); 
  voltage = getVoltage(); 
  current = getCurrent(); 
  power = voltage*current; 
  Serial.print(power); Serial.print("  "); Serial.print(energy); 
  Serial.print("  "); Serial.println(t); 
  String data = readBT(); 
  if(data.length() > 0 or millis() - lastDataTX >= SEND_DELTA) 
  { 
    BTSerial.print(t); BTSerial.print(","); BTSerial.println(energy); 
    lastDataTX = millis(); 
  } 
  if (data.length() == 0) 
  { 
    t = (millis() - t0)/1000; 
    energy += (current * (millis()-strt))/(float) 3600; 
    return; 
  } 
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
  t = (millis() - t0)/1000; 
  energy += (current * (millis()-strt))/(float) 3600; 
} 
/* 
   Code taken from https://forum.arduino.cc/index.php?topic=93867.0 
   Code from comment by elan_arr 
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
float getCurrent() 
{ 
  currentRaw = SENSITIVITY*analogRead(CURRENT_PIN)  
    + (1-SENSITIVITY)*currentRaw; 
  float dat = (float) currentRaw/(float) 780; 
  return dat*0.3; 
} 
float getVoltage() 
{ 
  voltageRaw = SENSITIVITY*analogRead(VOLTAGE_PIN)  
    + (1-SENSITIVITY)*voltageRaw; 
  float dat = (float) voltageRaw/(float) 284; 
  return dat*5.15; 
} 
