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

 #include ".\ref\db.hpp"
 #include ".\ref\rfid.hpp"
 #include ".\ref\screen.hpp"
//#include "C:\Project_iot\SmartLendPegBoardhw\time.hpp"
 #include ".\ref\kp.hpp"
// #include "C:\Project_iot\SmartLendPegBoard\hw\sensor.hpp"


db _DB;
rfid _rfd;
screen _SC;
kp _KP(_SC);
//sensor _sensor;


unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
   SPI.begin(); 
  //_DB.connectToWifi("ICST", "arduino123");
  _rfd.init();
  _SC.init();
  _SC.clear();
 // _sensor.init(25,5);
  Wire.begin();
  Wire.setClock(400000);
 _KP.init();

  Serial.begin(9600);
  



}

void loop(){
    //Serial.println("ENTERED LOOP :), your code doesn't work losers.");
   // _SC.printStr(string("put your card on the reader please."));
    string uid = _KP.getUserId();  

    string cid = "error";
    while(cid == "error"){
      Serial.print("Reading card id from RFID");
      cid = _rfd.readCid();
    }
   // _SC.clear();
    Serial.println(cid.c_str());
    if(cid != "error"){
     // if(_DB.isNewUser(cid)){
        //_SC.enterId();
        string uid = _KP.getUserId();  
        Serial.println(uid.c_str());
        //_DB.addNewUser(cid, stoi(uid));
        Serial.println("added new user \n");
        delay(1000);
        //_SC.clear();

 //     }
      // else{
      Serial.println("USER ALREADY IN THE SYSTEM");
      _rfd.init();
       delay(1000);

      // }
    }
  delay(5);
}
