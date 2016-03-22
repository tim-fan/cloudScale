#ifndef PININTERFACEDIRECT_H
#define PININTERFACEDIRECT_H

// Class implements an interface to analogRead a pin directly
// (this is in contrast to 'PinInterfaceMux' which reads through
// a multiplexer)

#include "PinInterface.h"

class PinInterfaceDirect : public PinInterface
{
  public:
    //constructor - simply specify the pin to read, and scale factor to convert the analog read to volts
    PinInterfaceDirect(int pin, double scaleFactorToVolts);
    void init(void);
    int readPin(void);

  private:
    int _pin;
};

#endif /* PININTERFACEDIRECT_H */
