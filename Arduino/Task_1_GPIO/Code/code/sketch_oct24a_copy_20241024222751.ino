void setup() {
  pinMode(13,OUTPUT);//设置引脚13为输出
  pinMode(9,OUTPUT);//设置引脚9为输出
  pinMode(7,INPUT);//设置引脚7为输入
}

void loop() {
digitalWrite(13,HIGH);
delay(500);
digitalWrite(13,LOW);
delay(500);//实现板上Led以1hz切换

  if(digitalRead(7) == HIGH){
    digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    delay(500);
  }
else{
  digitalWrite(9,LOW);
}//实现不同引脚的状态相同
}
