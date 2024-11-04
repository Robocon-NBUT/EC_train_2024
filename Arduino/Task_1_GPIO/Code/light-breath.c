//呼吸灯
void setup()
{
  pinMode(3,OUTPUT);
  pinMode(13,OUTPUT);
}
void loop()
{
  for(int i = 0; i <= 100;i++)   //使用for循环将i从0逐渐加到100
  {
    analogWrite(13, i);  //将i的数值写入13号针脚
    analogWrite(3, i);
    delay(5);  //延时5毫秒
  }
  for(int i = 100; i >= 0; i--)   //使用for循环将i从100逐渐减到0
  {
    analogWrite(13, i);
    analogWrite(3, i);  //将i的数值写入3号针脚
    delay(5);  //延时5毫秒
  }
}