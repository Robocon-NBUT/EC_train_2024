void setup() {
  pinMode(2, INPUT);
  pinMode(9, OUTPUT);
}
void loop() {
  int a=digitalRead(2);
  if(a == HIGH)
  {
  digitalWrite(9, HIGH); 
  }
  else
  {                    
  digitalWrite(9, LOW);
  }               
}
