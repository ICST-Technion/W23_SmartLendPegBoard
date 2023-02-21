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

#include <vector>





// Insert your network credentials
#define WIFI_SSID "TechPublic"
#define WIFI_PASSWORD ""

 #include ".\ref\db.hpp"
 #include ".\ref\rfid.hpp"
 #include ".\ref\screen.hpp"
//#include "C:\Project_iot\SmartLendPegBoardhw\time.hpp"
 #include ".\ref\kp.hpp"
// #include "C:\Project_iot\SmartLendPegBoard\hw\sensor.hpp"
#include ".\ref\sensor.hpp"
Adafruit_PCF8574 pcf;
Adafruit_PCF8574 pcf2;

std::vector<sensor*> sensors;
std::vector<int> last(6); // initalize a vector of size 6
db _DB;
rfid _rfd;
screen _SC;
kp _KP(_SC);


unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
   SPI.begin(); 
  _DB.connectToWifi("Teba", "12345678");
  _rfd.init();
  _SC.init();
  _SC.clear();
  Wire.begin();
  Wire.setClock(400000);
 _KP.init();

  Serial.begin(9600);
    if (!pcf.begin(0x27, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1);
  }

      if (!pcf2.begin(0x21, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1);
  }


  for(int i=0;i<3;i++){
     sensors.push_back(new sensor());
    sensors[i]->init(i*2,i*2+1,&pcf);
  }
  for(int i=0;i<3;i++){
    sensors.push_back( new sensor());
    sensors[i+3]->init(i*2,i*2+1,&pcf2);
  }
  // get the current taste
  for(int i=0;i<6;i++){  
    last[i]=sensors[i]->status();
  }

}

void loop(){
    _SC.printStr(string("put your card on the reader please."));  
    string cid = "error";
    while(cid == "error"){
      //Serial.print("Reading card id from RFID");
      cid = _rfd.readCid();
    }
    _SC.clear();
    Serial.println(cid.c_str());
    if(_DB.isNewUser(cid)){
        _SC.enterId();
        string uid = _KP.getUserId(); //TODO: add prints on the screen
        Serial.println(uid.c_str());
        _DB.addNewUser(cid, stoi(uid));
        Serial.println("added new user \n");
    }else{
      if(!_DB.isUserApproved(cid)){
      _SC.printStr(string("You don't have a permission, enter your id please."));
      delay(6000);
      string uid = _KP.getUserId();
      _DB.addNewUser(cid, stoi(uid));
      return;  
      }
    }
    _SC.printStr(string("Access granted."));  
    std::vector<bool> v=_DB.getUserLentItems(cid);
    for(int i=0;i<6;i++){
      if(v[i]==true)sensors[i]->ledOn();
      else sensors[i]->ledOf();
    }
    _SC.clear();
    delay(100);
    
   delay(30000); //Waiting for the user to finish.
  //these operatioons are done after the locker is closed
  for(int i=0;i<6;i++){     
    if(sensors[i]->status()!=last[i]){
      //item i has been taken or returned :
      Serial.println("items state has changed");
      if(last[i]==0){
        //item has been taken
        last[i]=1;
       _DB.lendItem(cid, i);
      }else{
        //item has been returned 
        last[i]=0;
        _DB.returnItem(cid, i);
      }
    }
  }  
  delay(1000);
}
