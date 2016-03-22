#ifndef BATHROOMSCALE_H
#define BATHROOMSCALE_H

#include "SevenSeg.h"

//class for reading weight from the lcd of a bathroom scale
//note this code is very particular to my specific bathroom scale
//https://www.thewarehouse.co.nz/red/catalog/product/Living-%26-Co-Digital-Bathroom-Scale-Glass-Silver?SKU=1455424
class BathroomScale {

  public:
    //constructor. Need to provide interfaces to read 4 LCD digits
    BathroomScale(SevenSeg (&_digits)[4]);

    //intialisation (initialises the given pin interfaces)
    void init();

    //checks if the scale is active (on)
    bool scaleIsOn();

    //reads the current weight displayed on the scale's LCD
    //returns -1 if the scale is off
    double readWeight();
    

    //keep reading weight until the value is stabilised. Return the final value
    //Stable = value not changed more than 0.1 kg for 4 seconds
    double readStableWeight();

  private:
    SevenSeg _digits[4]; //represents the four digits of the LCD display
    
    //call a given function on all digits
    void callForAllDigits(void (*function)(SevenSeg&));
    
    // helper functions to call member functions on a given digit
    void initialiseDigit(SevenSeg& digit);
    void resetDigit(SevenSeg& digit);
    void triggerDigitRead(SevenSeg& digit);
    
    //helper functions to call member functions on all digits
    void resetAllDigits(void);
    void allDigitsReadPins(void);
    
    //attempts to read the current weight displayed on the scale's LCD
    //willl return -1 if the scale is off, or if read value is determined to be invalid
    double tryReadWeight();
    
};

#endif /* BATHROOMSCALE_H */
