#include "BathroomScale.h"

BathroomScale::BathroomScale(SevenSeg (&digits)[4]):
  _digits(digits)
{};

bool BathroomScale::scaleIsOn(){
  return _digits[0].waitForSync();
}

double BathroomScale::tryReadWeight(void){
  //get a weight reading from the scale.
  resetAllDigits();

  //wait to sync with lcd signal
  //if timeout happens, return -1 (invalid weight)
  if (!scaleIsOn()){
    //Serial.println("Scale is not activated");
    return -1;
  }
  
  //delay 1 ms
  //use busy waiting loop instead of 'delay', to 
  //prevent a yield() to background ESP8266 functions
  //(I don't know how long they take!)
  unsigned long startTime = micros();
  while (micros() - startTime < 1000) {}

  //sample pins for each digit four times, with 4ms spacing
  startTime = micros();
  for (int i = 0; i < 4; i++){
    allDigitsReadPins();
    while (micros() - startTime < (i+1)*4000){
      //busy waiting
    }
  }

  //Digits have read their control lines four times
  //they are now ready to be read

  //sum up all digit values to get weight
  //if any digits are -1, the weight reading is not valid
  double weight = 0;
  bool weightIsValid = true;

  //Serial.print("Digit val: ");
  for (int i = 0; i < 4; i++){
    int iDigitVal = _digits[i].getValue();
    //Serial.print(iDigitVal);
    weight += iDigitVal * pow(10, i-1);
    if (-1 == iDigitVal){
      weightIsValid = false;
    }
  }
  //Serial.println("");
  
  if (!weightIsValid){
    weight = -1;
  }
  //Serial.println(weight);
  return weight;
}

double BathroomScale::readWeight(){
  //Serial.println("Read weight");
  double weight = tryReadWeight();
  while(weight < 0){
    
    //abort if scale is no longer activated
    if (!scaleIsOn()){
      //Serial.println("Scale is not on");
      return -1;
    }

    //otherwise, keep trying to read a valid weight value
    weight = tryReadWeight();
  }
  //Serial.println("Valid weight found");
  return weight;
}

double BathroomScale::readStableWeight(){
  unsigned long startTime = millis();
  unsigned long settlingTime = 4000; //wait for 4 sec to determine if stable
  double currentWeight = -1;
  double newWeight;
  bool weightIsStable = false;
  byte ledState = LOW;
  
  while(!weightIsStable){

    //get the current weight reading
    newWeight = readWeight();

    //abort if scale is no longer on
    if (newWeight < 0){
      Serial.println("Scale is not on");
      return -1;
    }

    if (abs(newWeight - currentWeight) < 0.05){
      //Serial.println(String(millis() - startTime));
      if (((millis() - startTime) > settlingTime) && (currentWeight > 5)){
          weightIsStable = true;
          //Serial.println(currentWeight);
      }
    }
    else{
      ledState = ~ledState;
      digitalWrite(LED_BUILTIN, ledState);
      //Serial.println("Weight Changed");
      //Serial.println(newWeight);
      currentWeight = newWeight;
      startTime = millis();  
    }
    Serial.println(newWeight);
    delay(100);
  }
  return currentWeight;  
}

void BathroomScale::init(void){
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.println("All digits init");
  for (int i = 0; i < 4; i++){
    _digits[i].initialise();
  }
}

void BathroomScale::resetAllDigits(void){
  for (int i = 0; i < 4; i++){
    _digits[i].reset();
  }
}

void BathroomScale::allDigitsReadPins(void){
  for (int i = 0; i < 4; i++){
    _digits[i].readPins();
  }
}


