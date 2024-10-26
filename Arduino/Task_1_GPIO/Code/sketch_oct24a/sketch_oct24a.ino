void setup() {
  pinMode(9, OUTPUT);
  pinMode(2, INPUT);
}

void loop() {
  int inputState = digitalRead(2);
  digitalWrite(9, inputState);          
}

 