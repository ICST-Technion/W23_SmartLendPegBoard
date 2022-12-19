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

#include "C:\Users\adam1\SmartLendPegBoard\hw\db.hpp"
#include "C:\Users\adam1\SmartLendPegBoard\hw\rfid.hpp"
#include "C:\Users\adam1\SmartLendPegBoard\hw\time.hpp"


db _DB;
rfid _rfd;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  _DB.connectToWifi("ICST", "arduino123");
  _rfd.init();



}

void loop(){
  _rfd.readCard();
}
