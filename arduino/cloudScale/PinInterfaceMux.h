#ifndef PININTERFACEMUX_H
#define PININTERFACEMUX_H

// Class implements an interface to analogRead a pin via a mux
#include "PinInterface.h"
#include "MuxController.h"
#include <Arduino.h>

class PinInterfaceMux : public PinInterface
{
  public:
    //constructor - specify the mux channel to use, a scale factor for converting
    //analog read values to volts, and provide a controller
    //to interface with the mux
    PinInterfaceMux(uint8_t pinChannel, double scaleFactorToVolts, MuxController& muxController);
    void init(void);
    int readPin(void);

  private:
    uint8_t _pinChannel;
    MuxController& _muxController;
};

#endif /* PININTERFACEMUX_H */
