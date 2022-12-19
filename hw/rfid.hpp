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

#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
//#include <SPI.h> //library responsible for communicating of SPI bus
//used in authentication
MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
#define SS_PIN    21
#define RST_PIN   22
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define greenPin     12
#define redPin       32

bool first;
bool start;
string workerId;
MFRC522 mfrc522(SS_PIN, RST_PIN); 

class rfid{
  public:
  void init();
  void readCard();
  string readingData();

};

void rfid::init(){
  ///////////// RFID setup //////////////////////////
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  //pinMode(greenPin, OUTPUT);
  //pinMode(redPin, OUTPUT);
  
  // Init MFRC522
  mfrc522.PCD_Init(); 
  Serial.println("rfid init done \n");
  Serial.println();
  workerId = "";
  start = false;
  first = true;
  // start = false;
  //  ///////////////////// RFID reed //////////////////////////
  // if (mfrc522.PICC_IsNewCardPresent() && !start ) 
  // {   
  //   start = true;
  //   if ( ! mfrc522.PICC_ReadCardSerial()) 
  //    {
  //   //should add this new user 
  //   Serial.printf("WE CAN READ CARD YAY \n");
  //   }else {
  //   //user allready have permission , should track after items.
  //   Serial.printf(" OH NO!, can't read card! \n");
  //  }
  // }else if(!start){ Serial.printf("readCard error");delay(1000);}
  //  if(!start) return;
}
  

void rfid::readCard() {
  if(digitalRead(3)==LOW){
      start = false;
      first = true;
      digitalWrite(redPin,LOW);
  }
  ///////////////////// RFID reed //////////////////////////
  if (mfrc522.PICC_IsNewCardPresent() && !start ) 
  {   
    start = true;
    if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    Serial.printf("readCard error1 \n");
  }else { }
  }else if(!start){ Serial.printf("readCard error2 \n");
    delay(1000);}
  if(!start) return;
  
  if(first){
    //////////////////////////////read rfid id///////////////////////////////////
    Serial.printf("rfid read: \n");
    workerId=readingData();
    Serial.printf("THE DATA WE READ: %s \n", workerId.c_str());
  }  
}

string rfid::readingData(){
  //prints the technical details of the card/tag
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); 
  
  //prepare the key - all keys are set to FFFFFFFFFFFFh
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  //  key.keyByte[0] = 0xB0;
  //  key.keyByte[1] = 0xB1;
  //  key.keyByte[2] = 0xB2;
  //  key.keyByte[3] = 0xB3;
  //  key.keyByte[4] = 0xB4;
  //  key.keyByte[5] = 0xB5;

  //buffer for read data
  byte buffer[SIZE_BUFFER] = {0};
 
  //the block to operate
  byte block = 1;
  byte size = SIZE_BUFFER;//</p><p>  //authenticates the block to operate
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
//    digitalWrite(redPin, HIGH);
//    delay(1000);
//    digitalWrite(redPin, LOW);
    return "";
  }

  //read data from block
  status = mfrc522.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
//    digitalWrite(redPin, HIGH);
//    delay(1000);
//    digitalWrite(redPin, LOW);
    return "";
  }
  else{
//      digitalWrite(greenPin, HIGH);
//      delay(1000);
//      digitalWrite(greenPin, LOW);
  }

//  Serial.print(F("\nData from block ["));
//  Serial.print(block);Serial.print(F("]: "));

 //prints read data
  for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++)
  {
//      Serial.write(buffer[i]);
  }
//  Serial.println(" ");

  string temp=string((char*)buffer+1);
  temp.erase(4);
  return temp;
}

