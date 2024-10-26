void setup() {
  // put your setup code here, to run once:
  pinMode(8,INPUT); 
  digitalWrite(13,HIGH);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  int a=digitalRead(8);
  if (a==HIGH) {
    digitalWrite(13,HIGH);
  }else{
    digitalWrite(13,LOW);
  }
}
