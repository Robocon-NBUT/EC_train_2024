#include<Arduino.h>
void setup() {
pinMode(4,INPUT);
pinMode(9,OUTPUT);
}

void loop() {
if(digitalRead(4)==HIGH)
  {
     digitalWrite(9,HIGH);
     delay(500);
     digitalWrite(9,LOW);
     delay(500);

  }
else
  {
    digitalWrite(9,LOW);
  }
}
