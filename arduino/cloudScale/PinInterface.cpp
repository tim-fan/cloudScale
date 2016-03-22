#include "PinInterface.h"
#include <math.h>
int PinInterface::readPinRoundedVolts(){
  double volts = readPin() * _scaleFactorToVolts;
  return int(volts + 0.5);
}
