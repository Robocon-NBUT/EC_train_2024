//点灯
//setup是初始化函数
void setup() {
pinMode(13,output);//pinMode是Arduino提供的库，用于设置引脚输入输出状态
}
//loop是循环函数
void loop() {
digitalWrite(13,HIGH);//digitalWrite是Arduino提供的库，用于设置输出引脚的高低电平均状态
delay(500);//delay是Arduino提供的库，用于设置占用单片机的时间，即延迟
dugitalWrite(13,LOW);
delay(500);
}
//steup和loop函数一定要有，空的也可以，不然会报错