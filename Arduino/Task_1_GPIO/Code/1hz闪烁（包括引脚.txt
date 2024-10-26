void setup() {
  Serial.begin(9600);
  pinMode(9, INPUT);
  pinMode(13, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
} 

void loop() {


  int sensorVal = digitalRead(2);

  digitalWrite(LED_BUILTIN, HIGH);  
  delay(1000);                      
  digitalWrite(LED_BUILTIN, LOW);   
  delay(1000); 

  if (sensorVal == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }   
}