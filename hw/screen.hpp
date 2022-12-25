#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

class  screen{

 public:
 void init();
 void enterId();
 void clear();

};

void screen::init(){

    Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
}

void screen::enterId(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10); 
     // Display static text
  display.println("Enter your ID please :)");
  display.display();
}

void screen::clear(){
  display.clearDisplay();
}

