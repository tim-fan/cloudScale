#include "MuxController.h"
#include <Arduino.h>

MuxController::MuxController(int pinInput, int pinS1, int pinS2, int pinS3) :
  _pinInput(pinInput),
  _pinS1(pinS1),
  _pinS2(pinS2),
  _pinS3(pinS3),
  _currentChannel(9) //not a valid channel - indicates unitialised channel
{};

void MuxController::init(void){
  pinMode(_pinS1, OUTPUT);  
  pinMode(_pinS2, OUTPUT);  
  pinMode(_pinS3, OUTPUT);
  pinMode(_pinInput, INPUT);
  Serial.println("Mux intitialised");
};


int MuxController::readChannel(uint8_t channel){

  setChannel(channel);
  
  //read the mux output
  int outputVal = analogRead(_pinInput);
  return outputVal;
  
};

void MuxController::setChannel(uint8_t channel){
  //if already on that channel, do nothing
  /*
  Serial.print("Read on channel: ");
  Serial.println(channel);
  Serial.print("Currently on channel: ");
  Serial.println(_currentChannel);
  */
  if (_currentChannel == channel){
    //Serial.println("Already on channel");
    return;
  }
  //otherwise:
  
  //determine select pin settings
  bool pinS1Setting = bitRead(channel, 0);
  bool pinS2Setting = bitRead(channel, 1);
  bool pinS3Setting = bitRead(channel, 2);
  /*Serial.print(_pinS1);
  Serial.print(":");
  Serial.print(pinS1Setting);
  Serial.print("\t");
  Serial.print(_pinS2);
  Serial.print(":");
  Serial.print(pinS2Setting);
  Serial.print("\t");
  Serial.print(_pinS3);
  Serial.print(":");
  Serial.println(pinS3Setting);
  */
  
  //set the select pins
  digitalWrite(_pinS1, pinS1Setting);
  digitalWrite(_pinS2, pinS2Setting);
  digitalWrite(_pinS3, pinS3Setting);  
  _currentChannel = channel;
  
  //MC54HC4051N datasheet shows channel-select to analog out 
  //propagation delays of up to 550ns around the expected 
  //operating conditions (Vcc = 3.3V)
  //Give the device about 1-2 microseconds to settle.
  unsigned long delayStartTime = micros();
  while ((micros() - delayStartTime) < 2){
    //busy wating
    //didn't use a 'delay' function, as I
    //don't want to pass control to background
    //utility functions when running on 
    //ESP8266 
  }  
};


