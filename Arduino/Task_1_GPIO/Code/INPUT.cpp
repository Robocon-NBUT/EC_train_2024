int ledPin = 9; 

void setup() {
   pinMode(ledPin, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH); 
  digitalWrite(ledPin, HIGH); 
  delay(1000);                     
  digitalWrite(LED_BUILTIN, LOW);   
  digitalWrite(ledPin, LOW);  
  delay(1000);                     
}