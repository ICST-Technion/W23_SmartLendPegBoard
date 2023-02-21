/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-keypad-lcd
 */

//#include <Keypad.h>
// #include <LiquidCrystal_I2C.h>
 #include <string.h>
// #include "Wire.h"
 #include "I2CKeyPad.h"

// using std::string;

// #define ROW_NUM     4 // four rows
// #define COLUMN_NUM  4 // four columns

// char keys[ROW_NUM][COLUMN_NUM] = {
//   {'1','2','3', 'A'},
//   {'4','5','6', 'B'},
//   {'7','8','9', 'C'},
//   {'*','0','#', 'D'}
// };
const uint8_t KEYPAD_ADDRESS = 0x20;
 
I2CKeyPad keyPad(KEYPAD_ADDRESS);
 
uint32_t start, stop;
uint32_t lastKeyPressed = 0;

byte pin_rows[ROW_NUM]      = {21, 19, 18, 5}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = {27, 14, 12, 13};   // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

int cursorColumn = 0;





class Keypad {
    public:
    void Init (){
  Serial.begin(9600);
  Serial.println(__FILE__);
 
  Wire.begin();
  Wire.setClock(400000);
  if (keyPad.begin() == false)
  {
    Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
    while(1);
  }
 
  keyPad.setKeyPadMode(I2C_KEYPAD_4x4);
    }
    string getUserId(){
        string output;
        int counter=0;
        while (counter<9){      
            char key = keypad.getKey();

            if (key) {
                counter++;
                output+=key;
                // lcd.setCursor(cursorColumn, 0); // move cursor to   (cursorColumn, 0)
                // lcd.print(key); 
            }

        }
        return output;
    }
}