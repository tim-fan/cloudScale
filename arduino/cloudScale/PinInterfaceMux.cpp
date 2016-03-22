#include "PinInterfaceMux.h"

PinInterfaceMux::PinInterfaceMux(uint8_t pinChannel, double scaleFactorToVolts, MuxController& muxController):
  _pinChannel(pinChannel),
  _muxController(muxController)
{
  _scaleFactorToVolts = scaleFactorToVolts;  
};

void PinInterfaceMux::init(void){
  _muxController.init();
  Serial.print("pinInterface init on mux channel ");
  Serial.println(_pinChannel);
};

int PinInterfaceMux::readPin(void){
  return _muxController.readChannel(_pinChannel);
};
