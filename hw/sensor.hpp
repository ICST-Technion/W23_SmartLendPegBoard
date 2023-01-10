// Endstop
int endstop_1 = 25;
int endstop_2 = 5;
int ledPin = 25;
unsigned long pm =0 ;
unsigned long interval = 0;
unsigned long  ledState = LOW;
void setup() {
    pinMode(endstop_1, INPUT);
    pinMode(endstop_2, OUTPUT);
    Serial.begin(9600);
}

void loop() {
  unsigned long cM = millis ();
  if (ledState == LOW){
    digitalWrite(ledPin , HIGH);
    //bS=digitalRead(18);
    digitalWrite(ledPin, LOW);
    delay(5);
  }else {
    //bS=digitalRead(18);
  }
  if(cM-pm>=interval){
    pm=cM;
    if(ledState ==LOW){
      ledState = HIGH;
    }else {
      ledState = LOW;
    }
    digitalWrite(ledPin,ledState);
  }
}