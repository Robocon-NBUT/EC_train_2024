void setup() {
  pinMode(3,OUTPUT);

}

void loop() {
  for(int i = 0; i < 200; i ++){
    analogWrite(3,i);
    delay(10);
  }
  for(int j = 255; j > 0; j --){
  analogWrite(3,j);
  delay(10);
  }

}
