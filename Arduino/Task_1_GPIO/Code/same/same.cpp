void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(13,OUTPUT);

}

void loop() {
  digitalWrite(3,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(3,LOW);
  digitalWrite(9,LOW);
  digitalWrite(13,LOW);
  delay(1000);
  // put your main code here, to run repeatedly:

}
