// Include the ESP8266 WiFi library. (Works a lot like the
// Arduino WiFi library.)
#include <ESP8266WiFi.h>
// Include the SparkFun Phant library.
#include <Phant.h>

//////////////////////
// WiFi Definitions //
//////////////////////

//add correct ssid and password here:
const char* ssid = "ssid";
const char* password = "pwd";

////////////////
// Phant Keys //
////////////////
const char PhantHost[] = "data.sparkfun.com";
const char PublicKey[] = "mKEvnrmWD0iKXbbN76r3";
const char PrivateKey[] = ""; //add key before using sketch

char terminator = char(0);

void sendSerialMessage(String message)
{
  //send a given string to arduino over serial.
  //message will be null terminated
  String toSend = message + String(terminator);
  Serial.print(toSend);
}

void setup() 
{
  Serial.begin(9600);
  connectWiFi();
  //pinMode(BUILTIN_LED, OUTPUT);
}

byte LED_STATE = HIGH;
void loop() 
{
  //wait for some serial data
  
  while (!Serial.available()) { 
    //LED_STATE = !LED_STATE;
    //digitalWrite(BUILTIN_LED, LED_STATE);
    delay(50);
  }
  

  //read to the newline characted
  String received = Serial.readStringUntil(terminator);

  //Assume received data is a weight value to send
  //to phant
  //sendSerialMessage(String("GOT: ") + received);
  Serial.print('C');
  postToPhant(received.toFloat());
}

void connectWiFi()
{
  byte ledStatus = LOW;

  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  //sendSerialMessage(("WFC: ") + String(ssid)); //wifi connect
  Serial.print('A');
  WiFi.begin(ssid, password);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
//    digitalWrite(BUILTIN_LED, ledStatus); // Write LED high/low
//    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;

    // Delays allow the ESP8266 to perform critical tasks
    // defined outside of the sketch. These tasks include
    // setting up, and maintaining, a WiFi connection.
    //sendSerialMessage("waiting for wifi connection");
    delay(500);
    // Potentially infinite loops are generally dangerous.
    // Add delays -- allowing the processor to perform other
    // tasks -- wherever possible.
  }
  //sendSerialMessage("WFC Done"); //wifi connect done
  Serial.print('B');
}


int postToPhant(float weight)
{
//  digitalWrite(BUILTIN_LED, HIGH);
  //sendSerialMessage(String("Log: ") + String(weight));
//  digitalWrite(BUILTIN_LED, LOW);
//  return 1;
  
  // LED turns on when we enter, it'll go off when we 
  // successfully post.
//  digitalWrite(BUILTIN_LED, HIGH);

  // Declare an object from the Phant library - phant
  Phant phant(PhantHost, PublicKey, PrivateKey);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String postedID = "Thing-" + macID;

  // Add the four field/value pairs defined by our stream:
  phant.add("id", postedID);
  phant.add("weight", weight);
  //phant.add("time", millis());

  // Now connect to data.sparkfun.com, and post our data:
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(PhantHost, httpPort)) 
  {
    // If we fail to connect, return 0.
    //sendSerialMessage("Connect Failed");
    Serial.print('D');
    return 0;
  }
  // If we successfully connected, print our Phant post:
  client.print(phant.post());

  //wait for client response
  unsigned long startTime = millis();
  while (!client.available() && (millis() < startTime + 5000)){
    delay(100);  
  }
  if (!client.available()){
    //sendSerialMessage("Phant no response");  
    Serial.print('E');
  }
  

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    //sendSerialMessage(String("Phant: " + line));
    //trying not to use serial
  }

  // Before we exit, turn the LED off.
//  digitalWrite(BUILTIN_LED, LOW);
  //sendSerialMessage("PhantPost: done");
  Serial.print('F');
  return 1; // Return success
}

