// Endstop
#include <Arduino.h>
#include <Adafruit_PCF8574.h>
class sensor {
  public :
  Adafruit_PCF8574 pcf;
  int endstop_1 = 25;// input green wire
  int endstop_2 = 5; // output red wire
  unsigned long  ledState = HIGH;
  void init(int endstop_1,int endstop_2);
  bool status () ;
};



unsigned long  ledState = HIGH;
void sensor::init(int endstop_1, int endstop_2,Adafruit_PCF8574& pcf) {
    this->endstop_1=endstop_1;
    this->endstop_2=endstop_2;
    this->ledState=HIGH;
    pcf.pinMode(endstop_1, INPUT);
    pcf.pinMode(endstop_2, OUTPUT);
    pcf.digitalWrite(endstop_2,HIGH);
    Serial.begin(9600);
}

 bool sensor:: status() {
  unsigned long bS = 0;

  if (ledState == LOW){
    pcf.digitalWrite(endstop_2 , HIGH);
    delay(100);
    bS=pcf.digitalRead(this->endstop_1);    
    digitalWrite(endstop_2, LOW);
    delay(100);
  }else {
    delay(100);
    bS=digitalRead(this->endstop_1);
  }

  if ( bS == LOW){
    ledState = LOW;
    digitalWrite(endstop_2, ledState);
    return true; // there is a tool
  }else {
    ledState = HIGH;
    digitalWrite(endstop_2, ledState);
    return false; // there is not a tool
  }
}