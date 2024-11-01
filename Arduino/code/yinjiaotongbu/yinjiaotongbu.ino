void setup() {
  // put your setup code here, to run once:
  pinMode(9,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(13,OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(9,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(1,HIGH);
  delay(1000);
  digitalWrite(9,LOW);
  digitalWrite(13,LOW);
  digitalWrite(1,LOW);
  delay(1000);  
  


}
