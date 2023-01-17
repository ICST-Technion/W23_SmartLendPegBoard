#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire,OLED_RESET);

class  screen{

 public:
 void init();
 void enterId();
 void clear();
 void printKey(char key);
 void printId(string id);
 void printStr(string str);

};

void screen::init(){

    Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
}

void screen::enterId(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10); 
     // Display static text
  display.println("Enter your ID please :) \n");
  display.display();
}

void screen::clear(){
  display.clearDisplay();
}

void screen::printKey(char key){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10); 
     // Display static text
  display.setCursor(0, 10); 
  display.println(key);
  display.display();
}

void screen::printId(string id){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10); 
     // Display static text
  display.setCursor(0, 10); 
  display.println(id.c_str());
  display.display();
}

void screen::printStr(string str){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10); 
     // Display static text
  display.setCursor(0, 10); 
  display.println(str.c_str());
  display.display();
}