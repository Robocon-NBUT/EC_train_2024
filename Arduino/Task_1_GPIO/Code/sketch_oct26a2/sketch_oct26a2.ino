void setup() {
  pinMode(2, INPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  int inputState = digitalRead(2);
  digitalWrite(9, inputState);
}