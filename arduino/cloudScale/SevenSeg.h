#ifndef SEVENSEG_H
#define SEVENSEG_H

//Handles interpretation of 7 segment display by reading two control pins
//Usage - instance must call 'readPins' to read each of the control lines 4 times, then it can return the 
// corresponding value for those control signals. Then call 'reset', and repeat.
// Timing of the reading is handled externally, for my unit the control lines 
// update each 4ms.
// Note, pin1/pin2 designation is arbitrary, but needs to be correct. For my unit
// pin1 is the more-left pin of the two relevant control lines, when looking at the
// underside of the scale PCB, with the lcd pads towards you.

#include <Arduino.h>
#include "PinInterface.h"

class SevenSeg {
  public:
    SevenSeg(PinInterface* pin1Interface, PinInterface* pin2Interface);
    PinInterface* _pin1Interface;
    PinInterface* _pin2Interface;
    void initialise(); //Call me during setup, to set pin modes
    bool waitForSync();//Blocks until ready to read pins (waits for start of sequence). Returns false if 1 second timeout is reached
    void readPins();   //Needs to be called 4 times (with correct timing) before the signal can be decoded
    int getValue();    // When ready to decode, call this to get the int value on the 7-segment. Returns -1 for "can't decode"
    void reset();      // clears all previous read values, read to read and decode again

  private:
    bool _pin1Vals[4]; //holds the 4 values read off the control lines, for pin 1
    bool _pin2Vals[4]; // ditto for pin 2
    int _readCount;    // maintains how many reads since the last reset
};

#endif /* SEVENSEG_H */
