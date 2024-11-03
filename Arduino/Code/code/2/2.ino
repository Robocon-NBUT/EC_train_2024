void setup() {
  pinMode(3,INTPUT);//设置引脚3为输入
  pinMode(9,OUTPUT);//设置引脚9为输出
}

void loop() {
int open =digitalRead(3);//将引脚3状态写入open
digitalWrite(9,open);//将open写入引脚9，实现引脚3和引脚9的状态同步
}
