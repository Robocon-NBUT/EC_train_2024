void setup() {
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);

}
void loop() {
  digitalWrite(10, LOW); 
  digitalWrite(5, LOW); 
  digitalWrite(13, HIGH);
  delay(5000);
  for(int i=0;i<3;i++){
  digitalWrite(13,LOW);    
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  }
  digitalWrite(13, LOW); 
  digitalWrite(5, HIGH);
  delay(3000); 
  digitalWrite(10, HIGH);
  digitalWrite(5, LOW); 
  digitalWrite(13, LOW);
  delay(5000);  
  for(int i=0;i<3;i++){
  digitalWrite(10,LOW );    
  delay(500);
  digitalWrite(10, HIGH );
  delay(500);
  } 

}