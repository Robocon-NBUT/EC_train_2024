void setup() {
  pinMode(3, INPUT); 
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(4, HIGH);
  int open = digitalRead(3); 
  digitalWrite(5, open);
}