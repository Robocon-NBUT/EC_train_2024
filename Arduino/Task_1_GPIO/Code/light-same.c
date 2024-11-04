//同步2和9
void setup() {
  // put your setup code here, to run once:
pinMode(2,INPUT);
pinMode(9,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int A = digitalRead(2);
if(A == HIGH)
{
digitalWrite(9,HIGH);
}
else
{
  digitalWrite(9,LOW);
}
}