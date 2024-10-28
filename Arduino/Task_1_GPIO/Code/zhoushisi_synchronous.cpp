void setup() 
{ 
  pinMode(10,INPUT);
  digitalWrite(9,OUTPUT);
  pinMode(9,OUTPUT);
}

void loop() 
{
  int a=digitalRead(10);
  if(a==HIGH)
  {
    digitalWrite(9,HIGH);
  }
  else
  {
    digitalWrite(9,LOW);
  }
}
