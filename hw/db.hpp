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
#define LENT "lent"
#define RETURNED "returned"

using namespace std;

class db{
    public:
    
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    bool connectToWifi(std::string Wifi_ssid, std::string wifi_password);
    bool addNewUser(string cid, int uid);
    std::string getFieldByPayload(std::string payload, std::string fieldType, std::string fieldName);
    std::string getField(std::string documentPath, std::string fieldType, std::string fieldName);
    void addField(FirebaseJson *content, string fieldName, string fieldType, string fieldValue);
    void updateDocumentField(string documentPath, string fieldName, string fieldType, string newData);
    vector<bool> getUserLentItems(string cid); //return the id of the item.
    bool readItem(string cid, int item_idx);
    void returnItem(string cid, int item_idx);
    void lendItem(string cid, int item_idx);
    int  getUidByCid(string cid);
    void addToLog(string cid, int item_idx, string action, string time_stamp);
    bool isUserApproved(string cid);
    bool isNewUser(string cid);
    string parseCid(string cid);
};

int stringToNum(string str) {
  int num=0,len=0;
  while(str[len]!='\0'){
    len++;
  }
  if(len==2) {
      num += (str[0]-'0')*10;
      num += (str[1]-'0');
  } else {
      num+=(str[0]-'0');
  }
  return num;
}

bool db::connectToWifi(std::string Wifi_ssid, std::string wifi_password){
        Serial.println("Got to this stage");
        Serial.begin(9600);
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


bool db::addNewUser(string cid, int uid){
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){

      cid = parseCid(cid);
      std::string documentPath = "USERS/" + cid;

      FirebaseJson content;

      content.set("fields/UID/doubleValue", std::to_string(uid).c_str());
      content.set("fields/Approved/booleanValue", "false");

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
  //Serial.printf("payload is: \n%s\n\n", fbdo.payload().c_str());
  Serial.printf("content: \n%s\n\n", content.to<std::string>().c_str());
  return content.to<std::string>().c_str();
}

vector<bool> db::getUserLentItems(string cid){
  cid = parseCid(cid);
  vector<bool> lent_items(ITEMS_CNT, false);
  for (size_t i = 0; i < ITEMS_CNT; i++)
  {
    lent_items[i] = readItem(cid, i);
  }
  return lent_items;
}

bool db::readItem(string cid, int item_idx){
  cid = parseCid(cid);
  std::string documentPath = "USERS/" + cid + "/" ;
  std::string field_name = "item" + std::to_string(item_idx);
  std::string payload =  getField(documentPath, "boolean", field_name);
  bool return_val = (payload == "true") ? true : false;
  return return_val;
}

void db::updateDocumentField(string documentPath, string fieldName, string fieldType, string newData)
{
  if (Firebase.ready())
  {
      FirebaseJson content;
      addField(&content, fieldName, fieldType, newData);
      if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), fieldName))
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      else
          Serial.println(fbdo.errorReason());
  }
}

void db::addField(FirebaseJson *content, string fieldName, string fieldType, string fieldValue)
{
  string field_path = "fields/"+fieldName+"/"+fieldType+"Value";
  content->set(field_path.c_str(), fieldValue.c_str());
}

void db::returnItem(string cid, int item_idx){
  cid = parseCid(cid);
  bool is_returned = !readItem(cid, item_idx);
  if(is_returned){
    Serial.println("item already returned! \n");
    return;
  }
  string doc_path = "USERS/" +cid + "/" ;
  updateDocumentField(doc_path, ("item" + to_string(item_idx)), "boolean", "false");
  addToLog(cid, item_idx, RETURNED, "time_stamp");
  //TODO: add timestamp
  //TODO: update the items collection when you do it.
}

bool db::isUserApproved(string cid){
  cid = parseCid(cid);
  std::string documentPath = "USERS/" + cid + "/" ;
  std::string field_name = "Approved";
  std::string payload =  getField(documentPath, "boolean", field_name);
  return (payload == "true") ? true : false;
}

void db::lendItem(string cid, int item_idx){

  cid = parseCid(cid);
  bool is_lent = readItem(cid, item_idx);
    if(is_lent){
    Serial.println("item already lent! \n");
    return;
  }
  string doc_path = "USERS/" + cid + "/" ;
  updateDocumentField(doc_path, ("item" + to_string(item_idx)), "boolean", "true");
  addToLog(cid, item_idx, LENT, "time_stamp");
  //TODO: add timestamp
  //TODO: update the items collection when you do it.
}

int db::getUidByCid(string cid){
  cid = parseCid(cid);
  std::string documentPath = "USERS/" + cid + "/" ;
  std::string field_name ="UID";
  std::string payload =  getField(documentPath, "double", field_name);
  return stringToNum(payload);
}

bool db::isNewUser(string cid){
  cid = parseCid(cid);
  string documentPath = "USERS/" + cid;
  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str())){
      return false;
    }
  return true;
}

void db::addToLog(string cid, int item_idx, string action, string time_stamp){ 
  cid = parseCid(cid);
  int uid = getUidByCid(cid);
  if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
      std::string documentPath = "LOG/";
      string msg = to_string(uid) + "  has " + action + " the Item " + to_string(item_idx) + "  at: ";
      // TODO:: add time stamp to the msg.
      FirebaseJson content;

      content.set("fields/UID/doubleValue", std::to_string(uid).c_str());
      content.set("fields/CID/stringValue", cid.c_str());
      //content.set("fields/time/timestampValue", time_stamp.c_str());
      content.set("fields/action/stringValue", action.c_str());      
      content.set("fields/item_idx/doubleValue", to_string(item_idx).c_str());
      content.set("fields/msg/stringValue", msg.c_str());
    
      if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
  }
}


string db::parseCid(string cid){
  cid.erase(std::remove_if(cid.begin(), cid.end(), ::isspace), cid.end());
  return cid;
}



