
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

int ledState = false;
char terminator = char(0);

void checkIncommingEspSerial()
{
  //checks for serial data from the ESP.
  //if there is data available, it is 
  //printed to the hardware serial.
  //It is assumed messages are sent from the ESP 
  //with a null char terminator.
  char received;
//  Serial.println("ESP serial check");
//  String message = "";
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
//    received = softSerial.read();
//    if (received == '\0')
//    {
//      Serial.println(message);
//      message = String();  
//    }
//    
//    message.concat(char(softSerial.read()));    
//    Serial.println("ESP serial read");
//    softSerial.setTimeout(1000);
//    received = softSerial.readStringUntil('Z');
//    Serial.println("ESP serial read complete");
//    Serial.println(received);
//    if (!received.endsWith(String(terminator)))
//    {
//      Serial.println("Incomplete message: no terminator received")  ;
//    }
//    delay(100);
//  }
//  Serial.println(message + String(" (unterminated)"));
//  Serial.println("ESP serial check complete");
//  return received;
}

void setup() {

  //debug info goes to Serial, 
  //weight values for logging go to ESP8266 via softSerial
  
  Serial.begin(9600);
  softSerial.begin(9600);
  scale.init();  
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(pinRst, OUTPUT);
  pinMode(pinBootMode, OUTPUT);
  digitalWrite(pinBootMode, HIGH);
  delay(500); //I don't really know how long these delays should be...
  digitalWrite(pinRst, LOW);
  delay(500);
  digitalWrite(pinRst, HIGH);
  delay(500);
}


void loop() {
  ledState = !ledState;
  
  //digitalWrite(LED_BUILTIN, analogRead(A0)>10);   
  //Serial.println(analogRead(A0));
  
//  //Voltage level testing
//  int vals[64];
//  pinInterface0.readPinRoundedVolts();
//  while(pinInterface0.readPinRoundedVolts() > 0) {}
//  while(pinInterface0.readPinRoundedVolts() <= 0)  {}
//  delay(1);
//  unsigned long startTime = micros();
//  for (int i = 0; i < 64; i++){
//    vals[i] = pinInterface0.readPinRoundedVolts();
////    vals[i] = pinInterface0.readPin();
//    while (micros() - startTime < (i+1)*1000){
//      //busy waiting
//    }
//  }
//  Serial.println(-1);
//  for (int i = 0; i < 64; i++){
//    Serial.println(vals[i]);
//  }
//  delay(1000);
//  Serial.println("End of sequence");
  
//
  float weight = scale.readStableWeight();
  Serial.println(weight);
  bool weightIsValid = weight >= 0;
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


