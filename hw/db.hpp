#include <vector>
#include <string>


#include <Arduino.h>
//#if defined(ESP32)
  #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
//#endif

#include <Firebase_ESP_Client.h>


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBZVl5UmOnKJhjq-sCDZ9Vv70ZvF6dZ39c"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "/" 

//insert project email and id
#define USER_EMAIL "smartlend.236333@gmail.com"
#define USER_PASSWORD "sadProject!"
#define FIREBASE_PROJECT_ID "smartlend-drawers"

#define ITEMS_CNT 8


class db{
    public:
    
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    bool connectToWifi(std::string Wifi_ssid, std::string wifi_password);
    bool addNewUser(int cid, int uid);
    std::string getFieldByPayload(std::string payload, std::string fieldType, std::string fieldName);
    std::string getField(std::string documentPath, std::string fieldType, std::string fieldName);
    void getUserLentItems(int cid); //return the id of the item.
    bool returnItem(int cid, int item);
    bool lendItem(int cid, int item);
    void initLog(int items_cnt);
};

bool db::connectToWifi(std::string Wifi_ssid, std::string wifi_password){
        Serial.println("Got to this stage");
        Serial.begin(115200);
        WiFi.begin(Wifi_ssid.c_str(), wifi_password.c_str());
        delay(250);
        Serial.print("Connecting to Wi-Fi");
        while (WiFi.status() != WL_CONNECTED){
            Serial.print(".");
            delay(300);
        }
        Serial.println();
        Serial.print("Connected with IP: ");
        Serial.println(WiFi.localIP());
        Serial.println();

        /* Assign the api key (required) */
        config.api_key = API_KEY;

        /* Assign the RTDB URL (required) */
      //  config.database_url = DATABASE_URL;

        /* Sign up */
        auth.user.email = USER_EMAIL;
        auth.user.password = USER_PASSWORD;
        config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
        
        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);
        Serial.print("DONE SETUP!\n");
        return true;
    }


bool db::addNewUser(int cid, int uid){
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
      std::string documentPath = "USERS/" + std::to_string(cid);

      FirebaseJson content;

      content.set("fields/UID/doubleValue", std::to_string(3).c_str());
      content.set("fields/Approved/doubleValue", std::to_string(0).c_str());

      //fill in the array of lent items, 1 for lent, 0 for non-lent.
      std::string lent_s;
      for (size_t i = 0; i < ITEMS_CNT; i++)
      {
        std::string field = "fields/item" + std::to_string(i) +"/booleanValue";
        content.set(field, false);
        lent_s = lent_s + "item" + std::to_string(i) + ",";

      }
      lent_s.pop_back();
      std::string edited = "UID,Approved," + lent_s; 
      

      if(Firebase.Firestore.patchDocument(&(this->fbdo), FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(),edited.c_str())){
        Serial.printf("ok\n%s\n\n", (this->fbdo).payload().c_str());
        return true;
      }else{
        Serial.println((this->fbdo).errorReason());
        return false;
      }
    return false;
    }
}

std::string db::getField(std::string documentPath, std::string fieldType, std::string fieldName)
{
  if (Firebase.ready())
  {
    if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str())){
      return getFieldByPayload(fbdo.payload().c_str(), fieldType, fieldName);
    }
    else
        Serial.println(fbdo.errorReason());
  }
  Serial.println("FIREBASE NOT READY \n");
  return "";
}

std::string db::getFieldByPayload(std::string payload, std::string fieldType, std::string fieldName)
{
  FirebaseJsonData content;
  FirebaseJson jsonContent;
  content.getJSON<std::string>(payload, jsonContent);
  jsonContent.get(content, "fields/"+fieldName+"/"+fieldType+"Value");
  Serial.printf("payload is: \n%s\n\n", fbdo.payload().c_str());
  Serial.printf("content: \n%s\n\n", content.to<std::string>().c_str());
  return content.to<std::string>().c_str();
}

void db::getUserLentItems(int cid){

  std::string documentPath = "USERS/1/" ;
  std::string payload =  getField(documentPath, "double", "UID");
  
  
}
