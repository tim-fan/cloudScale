


/*
 * 
 * Sets up a serial bridge to the ESP8266 connected to the arduino, to allow programming 
 * off the ESP8266 via a serial connection to the arduino.
 * Also sets the required pin configuration to enable the ESP8266 bootloader mode.
 *   
*/

//#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

//pins for software serial (serial connection to ESP8266)
const int pinRx = 3;
const int pinTx = 2;

//pins for controlling ESP8266 mode. Both are active low
const int pinBootMode = 5; //aka pin GPIO_0
const int pinRst = 6;

SoftwareSerial softSerial(pinRx, pinTx); // RX, TX
//AltSoftSerial altSerial;

void setup()
{
  // start serial port at 115200 bps:
  //Serial.begin(230400);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  softSerial.begin(9600);
  //altSerial.begin(9600);

  //put ESP8266 in bootloader mode
  pinMode(pinRst, OUTPUT);
  pinMode(pinBootMode, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(pinRst, LOW);
  digitalWrite(pinBootMode, LOW);
  delay(500); //I don't really know how long these delays should be...
  digitalWrite(pinRst, HIGH);  
  delay(500);
  digitalWrite(pinBootMode, HIGH);
  delay(500);
}

void loop() // run over and over
{
  //if (softSerial.overflow()) {
  // Serial.println("SoftwareSerial overflow!"); 
  //}
  while (softSerial.available())
  {
    char incomingByte = softSerial.read();
    if (incomingByte == 'A'){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
    }
    Serial.write(incomingByte);
  }
  
  while (Serial.available())
  {
    char incomingByte = Serial.read();      
    //Serial.write(incomingByte);
    softSerial.write(incomingByte);
  }
}
