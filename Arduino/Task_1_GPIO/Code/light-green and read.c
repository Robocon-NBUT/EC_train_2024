//红绿灯
void setup() {
pinMode(3,OUTPUT);
pinMode(5,OUTPUT);
pinMode(7,OUTPUT);
}

void loop() {
  //红亮5s
digitalWrite(3,HIGH);
delay(5000);
//红闪3s
digitalWrite(3,LOW);
delay(500);
for(int i=0;i<=3;i++)
{
digitalWrite(3,HIGH);
delay(500);
digitalWrite(3,LOW);
delay(500);
}
  //黄亮5s
digitalWrite(5,HIGH);
delay(5000);
//黄闪3s
digitalWrite(5,LOW);
delay(500);
for(int i=0;i<=3;i++)
{
digitalWrite(5,HIGH);
delay(500);
digitalWrite(5,LOW);
delay(500);
}

  //绿亮5s
digitalWrite(7,HIGH);
delay(5000);
//绿闪3s
digitalWrite(7,LOW);
delay(500);
for(int i=0;i<=3;i++)
{
digitalWrite(7,HIGH);
delay(500);
digitalWrite(7,LOW);
delay(500);
}
}
