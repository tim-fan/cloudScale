#ifndef PININTERFACE_H
#define PININTERFACE_H

//SevenSeg object needs to read two pins to determine
//the digit displayed. These pins can be read directly 
//via an analogRead on a specific pin, or indirectly through
//selection of a channel on a mux followed by an anolg read
//on the mux output.
// This class abstracts the pin interface to support both options.
// The two options are then implemented in derived classes

class PinInterface
{
  public:
    virtual void init(void) = 0;
    virtual int readPin(void) = 0;  
    int readPinRoundedVolts(void);
  protected:
    double _scaleFactorToVolts;
};

#endif /* PININTERFACE_H */
