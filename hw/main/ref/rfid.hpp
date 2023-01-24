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
//Provide the RTDB payload printing info and other helper functions.

#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
//#include <SPI.h> //library responsible for communicating of SPI bus
//used in authentication
MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
// #define SS_PIN    21
// #define RST_PIN   22
// #define SIZE_BUFFER     18
// #define MAX_SIZE_BLOCK  16
// #define greenPin     12
// #define redPin       32

#define SS_PIN  5 // SCK-GPIO 18, MOSI-GPIO 17, MISO-GPIO 16, Vcc-3.3v, GND -GND,
#define RST_PIN 4
// Declaration for SSD1306 display connected using software I2C pins are(22 SCL, 21 SDA)
// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


using std::string;

bool first;
bool start;
string workerId;
MFRC522 mfrc522(SS_PIN, RST_PIN); 

class rfid{
  public:
  void init();
  // void readCard();
  // string readingData();
  string readCid();

};

void rfid::init(){
  ///////////// RFID setup //////////////////////////
  Serial.begin(9600);
  //SPI.begin(); // Init SPI bus
  //pinMode(greenPin, OUTPUT);
  //pinMode(redPin, OUTPUT);
  
  // Init MFRC522
  mfrc522.PCD_Init(); 
  Serial.println("rfid init done \n");
  Serial.println();
}

string rfid::readCid(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("there's no card present \n");
		return "error";
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
    Serial.println("can't read card! \n");
		return "error";
	}

  String userid;
  for (uint8_t i=0; i < 4; i++)
  {
    Serial.println("iteration\n");
    userid += "0x" + String(mfrc522.uid.uidByte[i], HEX) + " "; 
  }
  //string uid(userid);
  Serial.print("User ID: ");
  Serial.println(userid);
  string temp(userid.c_str());
  return temp;
  //return userid;
}

