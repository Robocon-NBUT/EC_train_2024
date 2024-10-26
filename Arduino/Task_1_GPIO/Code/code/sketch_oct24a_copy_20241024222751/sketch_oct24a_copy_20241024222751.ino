void setup() {
  pinMode(13,OUTPUT);//设置引脚13为输出
  pinMode(9,OUTPUT);//设置引脚9为输出
  pinMode(3,INPUT);//设置引脚3为输入
}

void loop() {
digitalWrite(13,HIGH);
delay(500);
digitalWrite(13,LOW);
delay(500);//实现板上Led以1hz切换

if(digitalRead(3)==HIGH){//实现两个引脚状态相同
for(int i=0;i<=255;i++)
{
  analogWrite(9,i);
  delay(5);
}
for(int i=255;i>=0;i--)
{
analogWrite(9,i);
delay(5);
}
}
else{
  digitalWrite(9,LOW);
  }//实现呼吸灯
}