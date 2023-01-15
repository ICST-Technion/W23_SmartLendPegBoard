// Endstop
int endstop_1 = 25;
int endstop_2 = 5;
unsigned long bS = 0;
unsigned long pm =0 ;
unsigned long interval = 25;
unsigned long  ledState = HIGH;
void setup() {
    pinMode(endstop_1, INPUT);
    pinMode(endstop_2, OUTPUT);
    digitalWrite(endstop_2,HIGH);
    Serial.begin(9600);
}

 void loop() {
  unsigned long cM = millis ();
  if (ledState == LOW){
    digitalWrite(endstop_2 , HIGH);
    bS=digitalRead(25);
    digitalWrite(endstop_2, LOW);
    delay(15);
  }else {
    bS=digitalRead(25);
  }
  if(cM-pm>=interval){
    pm=cM;
  //  delay(15);
    if(ledState ==LOW){
      ledState = HIGH;
    }else {
      ledState = LOW;
    }
    digitalWrite(endstop_2,ledState);
  }
}