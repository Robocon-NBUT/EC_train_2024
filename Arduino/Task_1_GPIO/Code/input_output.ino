void setup() {
  pinMode(3, INPUT); 
  pinMode(5, OUTPUT);
}

void loop() {
  int open = digitalRead(3); 
  digitalWrite(5, open);
}