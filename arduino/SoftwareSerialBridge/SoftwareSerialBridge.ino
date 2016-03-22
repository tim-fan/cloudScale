/*
  Serial Call and Response

  Wait for Rx data on hardware serial, and send it to software serial tx
  also,
  Wait for Rx data on software serial, and send it to hardware serial tx
  
*/
#include <SoftwareSerial.h>

const int pinRx = 3;
const int pinTx = 2;

SoftwareSerial mySerial(pinRx, pinTx); // RX, TX

void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  mySerial.begin(9600);
}
char newline[] = "\r\n";
bool testStringReceived;
String received;
const int pinBootMode = 5; //aka pin GPIO_0
const int pinRst = 6;
void loop() // run over and over
{
  if (mySerial.available())
  {
    Serial.println(String("Length before read: ") + String(mySerial.available()));
    received = mySerial.readStringUntil(*newline);
    testStringReceived = (received == String("test response"));
    Serial.println(String("Got response: ")+ received);
    if (testStringReceived)
    {
      Serial.println("success");
    }
    else
    {
      Serial.println("failure");
    }
    
    //Serial.write(mySerial.read());
    
//    pinMode(pinRst, OUTPUT);
//    pinMode(pinBootMode, OUTPUT);
//    pinMode(LED_BUILTIN, OUTPUT);
//    digitalWrite(LED_BUILTIN, HIGH);
//    digitalWrite(pinBootMode, LOW);
//    delay(500); //I don't really know how long these delays should be...
//    digitalWrite(pinRst, LOW);
//    delay(500);
//    digitalWrite(pinRst, HIGH);  
//    delay(500);
//    //digitalWrite(pinBootMode, HIGH);
//    delay(500);
  }
  if (Serial.available())
  {
    int incomingByte = Serial.read();
    //Serial.write(incomingByte);
    mySerial.write(incomingByte);
  }
}
