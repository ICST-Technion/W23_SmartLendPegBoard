/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
*/




// Insert your network credentials
#define WIFI_SSID "TechPublic"
#define WIFI_PASSWORD ""

#include "C:\Project_iot\SmartLendPegBoard\hw\db.hpp"
#include "C:\Project_iot\SmartLendPegBoard\hw\rfid.hpp"
//#include "C:\Project_iot\SmartLendPegBoard\hw\screen.hpp"
#include "C:\Project_iot\SmartLendPegBoard\hw\time.hpp"
#include "C:\Project_iot\SmartLendPegBoard\hw\kp.hpp"



db _DB;
rfid _rfd;
screen _SC;
kp _KP(_SC);


unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  _DB.connectToWifi("ICST", "arduino123");
  _rfd.init();
   _SC.init();
   _SC.clear();

  Serial.begin(9600);



}

void loop(){
  Serial.println("ENTERED LOOP :), your code doesn't work losers.");
  Serial.println("put your card please.");
  string cid = _rfd.readCid();
  Serial.println(cid.c_str());
  if(cid != "error"){
    if(_DB.isNewUser(cid)){
      _DB.addNewUser(cid, 2121);
      Serial.println("added new user \n");
    }else{
      Serial.println("user already exists.\n");
      _SC.enterId();
      
    }
  }
  Serial.println(_KP.getUserId().c_str());

}
