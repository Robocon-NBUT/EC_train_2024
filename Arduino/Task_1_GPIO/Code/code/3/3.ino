void setup() {
 pinMode(9,OUTPUT);//设置引脚9为输出
}

void loop() {
for(int i=0;i<=255;i++)//循环使i模拟量升高使灯变亮
{
  analogWrite(9,i);
  delay(5);
}
for(int x=0;x<=10;x++){
digitalWrite(9,HIGH);
delay(50);
digitalWrite(9,LOW);
delay(50);
}

for(int i=255;i>=0;i--)//循环使i模拟量降低使灯变暗
{
analogWrite(9,i);
delay(5);
}
delay(5);
}
