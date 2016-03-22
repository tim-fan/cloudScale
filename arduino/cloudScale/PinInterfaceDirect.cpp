#include "PinInterfaceDirect.h"
#include <Arduino.h>

PinInterfaceDirect::PinInterfaceDirect(int pin, double scaleFactorToVolts):
  _pin(pin)
{
  _scaleFactorToVolts = scaleFactorToVolts;
};

void PinInterfaceDirect::init(void){
  pinMode(_pin, INPUT);
};

int PinInterfaceDirect::readPin(void){
  return analogRead(_pin);
};


