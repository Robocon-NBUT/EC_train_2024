void setup() {
 pinMode(13,OUTPUT);//设置引脚13为输出
}

void loop() {
digitalWrite(13,HIGH);//设置引脚13为高电压，Led发光
delay(500);//亮500毫秒
digitalWrite(13,LOW);//设置引脚13为低电压，Led不发光
delay(500);//停500毫秒，实现1hz周期
}