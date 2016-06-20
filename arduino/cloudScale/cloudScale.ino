
#include "SevenSeg.h"
#include "MuxController.h"
#include "PinInterfaceMux.h"
#include "PinInterfaceDirect.h"
#include "BathroomScale.h"
#include <SoftwareSerial.h>

double scaleFactorToVolts = 3.3 / 1024;

//instantiate some pin interfaces for reading the sevenseg displays
PinInterfaceDirect pinInterface0(A0, scaleFactorToVolts);
PinInterfaceDirect pinInterface1(A1, scaleFactorToVolts);
PinInterfaceDirect pinInterface2(A2, scaleFactorToVolts);
PinInterfaceDirect pinInterface3(A3, scaleFactorToVolts);
PinInterfaceDirect pinInterface4(A4, scaleFactorToVolts);
PinInterfaceDirect pinInterface5(A5, scaleFactorToVolts);
PinInterfaceDirect pinInterface6(A6, scaleFactorToVolts);
PinInterfaceDirect pinInterface7(A7, scaleFactorToVolts);

//alternatively read pins through a mux
// MuxController muxController(A0,5,4,13);
//PinInterfaceMux pinInterface0(0, scaleFactorToVolts, muxController);
//PinInterfaceMux pinInterface1(1, scaleFactorToVolts, muxController);
//PinInterfaceMux pinInterface2(2, scaleFactorToVolts, muxController);
//PinInterfaceMux pinInterface3(3, scaleFactorToVolts, muxController);
//PinInterfaceMux pinInterface4(4, scaleFactorToVolts, muxController);
//PinInterfaceMux pinInterface5(5, scaleFactorToVolts, muxController);
//PinInterfaceMux pinInterface6(6, scaleFactorToVolts, muxController);
//PinInterfaceMux pinInterface7(7, scaleFactorToVolts, muxController);

//digits array, representing 4 seven-segment digits 
//on the scale's LCD
SevenSeg digits[4] {
    SevenSeg(&pinInterface7, &pinInterface6), //decimal point
    SevenSeg(&pinInterface1, &pinInterface0), // ones
    SevenSeg(&pinInterface5, &pinInterface4), // tens
    SevenSeg(&pinInterface3, &pinInterface2)  // hundreds
}; 

BathroomScale scale(digits);

//pins for software serial (serial connection to ESP8266)
//debug info goes to hardware Serial, 
//weight values for logging go to ESP8266 via softSerial
const int pinRx = 3;
const int pinTx = 2;
SoftwareSerial softSerial(pinRx, pinTx); // RX, TX

//Control pins for ESP8266 - used to reset the module and keep 
//it out of bootmode
const int pinBootMode = 5; //aka pin GPIO_0
const int pinRst = 6;   

char terminator = char(0);

void checkIncommingEspSerial()
{
  //checks for serial data from the ESP.
  //if there is data available, it is 
  //printed to the hardware serial.
  
  char received;
  if (softSerial.available())
  {
    Serial.print("From ESP8266: ");
  }
  while (softSerial.available())
  {
    received = softSerial.read();
    Serial.print(received);
  }
  Serial.println("");
}

void setup() {

  //start serial (software and hardware)
  Serial.begin(9600);
  softSerial.begin(9600);
  
  //run initialisation functions for the scale
  scale.init();  
  
  //set pin modes
  pinMode(pinRst, OUTPUT);
  pinMode(pinBootMode, OUTPUT);
  
  //reset ESP8266
  digitalWrite(pinBootMode, HIGH);
  delay(500); //I don't really know how long these delays should be...
  digitalWrite(pinRst, LOW);
  delay(500);
  digitalWrite(pinRst, HIGH);
  delay(500);
}


void loop() {
  //read weight from scale
  float weight = scale.readStableWeight();
  Serial.println(weight);
  bool weightIsValid = weight >= 0;
  
  //if valid weight received, send over soft-serial to ESP8266 for logging
  if (weightIsValid){
    Serial.println("Got stable weight!");
    Serial.println(weight);
    Serial.println("Sending to phant");
    
    softSerial.print(weight);
    softSerial.print(terminator);
  }
  
  checkIncommingEspSerial();
  delay(200);
}


