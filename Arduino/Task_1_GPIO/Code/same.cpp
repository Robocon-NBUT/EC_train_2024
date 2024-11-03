void setup() {
  pinMode(3, INPUT); 
  pinMode(9, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(4, HIGH);
  int open = digitalRead(3); 
  digitalWrite(9, open);
}
