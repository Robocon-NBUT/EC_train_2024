#include<Arduino.h>
 int light=0;
 int change=5;
void setup() {
   pinMode(9,OUTPUT);
}
void loop() {
    digitalWrite(13,LOW);
    analogWrite(9,light);
    light=light+change;
    if(light>=255 or light==0)
    {
      change=change*-1;
    }
    delay(30); 
}
