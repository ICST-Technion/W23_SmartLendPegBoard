#include <SPI.h>
#include "Wire.h"
#include "I2CKeyPad.h"

const uint8_t KEYPAD_ADDRESS = 0x20;
const char keys[19] = "12344568789BC0E NF";  // N = NoKey, F = Fail
I2CKeyPad keyPad(KEYPAD_ADDRESS);

 class kp {
     public:
     screen sc;
     kp(screen& sc);
     void init();
     string getUserId();
   

 };

  kp::kp(screen &sc) : sc(sc){}

  void kp::init(){
        if (keyPad.begin() == false) {
        Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
        while (1);
        }
 
        keyPad.setKeyPadMode(I2C_KEYPAD_4x4);
    }

string kp::getUserId()
{
    string output;
    int counter=0;
      uint32_t now = millis();
      uint32_t start, stop;
uint32_t lastKeyPressed = 0;

    // uint8_t index = 17;
    // char key = 'N';

    //     while (counter<9){      
    //         if(key == 'N' || key == 'F')
    //         {
    //             index = keyPad.getKey();
    //             key = keys[index];
    //             if (key ) {
    //                 counter++;
    //                 output+=key;
    //                 Serial.println(key);
    //             //    sc.printKey(key);
    //             }
    //             key = 'N';
    //         }
    // }

    // sc.clear();
    // Serial.println(output.c_str());
    // sc.printId(output);
    // return output;
    volatile bool keyChange = false;

    while(counter<9){
        if (now - lastKeyPressed >= 100)
        {
            lastKeyPressed = now;

            start = micros();
            uint8_t index = keyPad.getKey();
            output+=keys[index];

            stop = micros();

            Serial.print(millis());
            Serial.print("\t");
            Serial.print(index);
            Serial.print("\t");
            Serial.print(keys[index]);
            Serial.print("\t");
            Serial.println(stop - start);
            if(keys[index] != 'N')counter++;
            Serial.print("STUCK\n");
            now = millis();
        }

    }
    return output;
}