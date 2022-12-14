#include <vector>
#include <string>


#include <Arduino.h>
//#if defined(ESP32)
 // #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
//#endif

#include <Firebase_ESP_Client.h>


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#include "C:\Users\user\OneDrive\Desktop\SmartLendPegBoard\hw\db.hpp"

#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
//#include <SPI.h> //library responsible for communicating of SPI bus
//used in authentication
MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
#define SS_PIN    21
#define RST_PIN   22
MFRC522 mfrc522(SS_PIN, RST_PIN); 
bool start;


// // Insert Firebase project API Key
// #define API_KEY "AIzaSyBZVl5UmOnKJhjq-sCDZ9Vv70ZvF6dZ39c"

// // Insert RTDB URLefine the RTDB URL */
// #define DATABASE_URL "/" 

// //insert project email and id
// #define USER_EMAIL "smartlend.236333@gmail.com"
// #define USER_PASSWORD "sadProject!"
// #define FIREBASE_PROJECT_ID "smartlend-drawers"


void setup(){
  ///////////// RFID setup //////////////////////////
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  
  // Init MFRC522
  mfrc522.PCD_Init(); 
  Serial.println("Approach your reader card...");
  Serial.println();
  start = false;
   ///////////////////// RFID reed //////////////////////////
  if (mfrc522.PICC_IsNewCardPresent() && !start ) 
  {   
    start = true;
    if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    //should add this new user 
    Serial.printf("enter your ID to get approved");
  }else {
    //user allready have permission , should track after items.
    Serial.printf(" I have permission!");

   }
  }else if(!start){ Serial.printf("readCard error");delay(1000);}
   if(!start) return;
   }
  

  void loop() {
  
    if(digitalRead(3)==LOW){
        start = false;
        first = true;
        digitalWrite(redPin,LOW);
    }
    if(first){
        //////////////////////////////read rfid id///////////////////////////////////
        Serial.printf("rfid read: \n");
        workerId=readingData();
        Serial.printf(workerId.c_str());
        /////////////////////////////////////////////////////////////////////////////
        
        first = false;
        digitalWrite(redPin,HIGH);
       }
     }  