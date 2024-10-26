void setup() {
  // put your setup code here, to run once:
  pinMode(5,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=1;i<=255;i++){
    analogWrite(5,i);
    delay(10);
  }
  for(int i=255;i>0;i--){
    analogWrite(5,i);
    delay(10);
  }
}
