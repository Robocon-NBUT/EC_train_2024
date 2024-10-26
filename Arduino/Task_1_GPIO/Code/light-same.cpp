void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(9,OUTPUT);
}
void loop(){
  digitalWrite(LED_BUILTIN,HIGH);
  digitalWrite(9,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  digitalWrite(9,LOW);
  delay(500);  
}