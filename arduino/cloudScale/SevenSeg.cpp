#include "SevenSeg.h"
#include <Arduino.h>
SevenSeg::SevenSeg(PinInterface* pin1Interface, PinInterface* pin2Interface) :
  _pin1Interface(pin1Interface),
  _pin2Interface(pin2Interface)
  {
    reset(); 
};

void SevenSeg::initialise(){
  _pin1Interface->init();
  _pin2Interface->init();
}

void SevenSeg::reset(){
  _readCount = 0;
  for (int i = 0; i < 4; i++) {
    _pin1Vals[i] = false;
    _pin2Vals[i] = false;  
  }
}

bool SevenSeg::waitForSync(){
  //read pin1 repeatedly, until a transition from 
  //1 or 3 volts, to 0 or 2 volts.
  //return false if 1s timeout is reached
  int prevVal = -1;
  int currentVal = -1;
  unsigned long startTime = millis();
  while (!((prevVal == 1 || prevVal == 3) && (currentVal == 0 || currentVal == 2))){
    prevVal = currentVal;
    currentVal  = _pin1Interface->readPinRoundedVolts();
    if ((millis() - startTime) > 1000){
      return false; //timeout reached
    }
  }
  return true;
}

void SevenSeg::readPins(){
  //if have read four times already, do nothing (wait for reset)
  if (_readCount > 3){
    return;
  }

  //analog read
  _pin1Vals[_readCount] = _pin1Interface->readPinRoundedVolts() > 1;
  _pin2Vals[_readCount] = _pin2Interface->readPinRoundedVolts() > 1;  
  
  _readCount++;
}

int SevenSeg::getValue(){
  //overall process: concatenate read values from both pins
  //into one 8-bit binary code (saved as binary array).
  //Then convert this code to decimal.
  //Then lookup the decimal code in an array of codes, where
  //code position equals the corresponding digit.
  //(i.e. if code == digitCodes[i] then digit == i)
  
  //can only return a valid value if the control lines have
  //been read 4 times
  if (_readCount < 4) {
    return -1;
  }

  //concatenate read values to get binary code
  bool binaryCode[8];

  for (int i = 2; i >= 0; i--){
    //Serial.print(_pin2Vals[i]);
  }  
  for (int i = 3; i >= 0; i--){
    //Serial.print(_pin1Vals[i]);
  }

  //Serial.println("");
  
  for (int i = 0; i < 4; i++){
    binaryCode[i] = _pin1Vals[i];
    binaryCode[i+4] = _pin2Vals[i];
  }

  //convert binary code to decimal
  int decimalCode = 0;
  for (int i = 0; i < 7; i++){
    decimalCode = decimalCode * 2 + binaryCode[i];
  }
  //Serial.println(decimalCode);
  
  //convert code to digit via lookup
  int digitCodes[11] = {2, 122, 33, 48, 88, 20, 4, 58, 0, 16, 127};
  int digit = -1;
  for (int i = 0; i < 11; i++){
    if (decimalCode == digitCodes[i]){
      digit = i;
    }
  }
  // note - digit '10' refers to a blank. If this digit is read, treat it as zero
  if (10 == digit){
    digit = 0;
  }
  return digit;
}

