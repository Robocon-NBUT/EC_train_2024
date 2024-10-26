void setup (){
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(9,OUTPUT);
}
void loop(){
  for (int a=0; a<=255;a++)                
  {
    analogWrite(9,a);
    analogWrite(LED_BUILTIN,a);
    delay(8);                                       
  }
  for (int a=255; a>=0;a--)            
  {
    analogWrite(9,a);
    analogWrite(LED_BUILTIN,a);
    delay(8);                             
  }
}
