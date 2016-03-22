#ifndef MUXCONTROLLER_H
#define MUXCONTROLLER_H

#include <Arduino.h>

//Provides an interface to an 8-to-1 analog multiplexer. Has been coded/tested on MC54HC4051N
class MuxController {
  public:
    //constructor defines pin mapping for input and select pins
    //addressing of inputs uses select pins as 
    //3-bit integer, with pinS3 as most significant bit
    //e.g. s3=HIGH, s2=HIGH, s1=LOW
    //     is used to select input channel 6
    // The address channel is read via the specified 'pinInput' pin
    MuxController(int pinInput, int pinS1, int pinS2, int pinS3);

    //set pin-modes (call this in arduino's setup method)
    void init();

    //provided an unsigned int between 0 and 7, this function will set the 
    //select pins on the mux to select that channel, and perform an analog
    //read on the output, returning the result
    int readChannel(uint8_t channel);

  private:
    int _pinInput;
    int _pinS1;
    int _pinS2;
    int _pinS3;
    uint8_t _currentChannel;
    void setChannel(uint8_t channel);

};

#endif /* MUXCONTROLLER_H */
