
# 电控代码
LED灯以1赫兹的频率切换

~~~

void setup() {
    pinMode(9, OUTPUT); // 设置LED引脚为输出模式
}

void loop() {
    digitalWrite(9, HIGH); // 打开LED
    delay(1000);                 // 等待1000毫秒
    digitalWrite(9, LOW);  // 关闭LED
    delay(1000);                 // 等待1000毫秒
}
~~~
## 第二个
呼吸灯
~~~
void setup() {
  pinMode(9, OUTPUT);
}

void loop() {
  for(int i = 0; i <= 200; i++) {
    analogWrite(9, i);
    delay(5);
  }
  for(int i = 200; i >= 0; i--) {
    analogWrite(9, i);
    delay(5);
  }
}
~~~

# 第三个
输入使与另一个数字引脚同步 LED 状态。
~~~
void setup() {
  pinMode(2, INPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  int inputState = digitalRead(2);
  digitalWrite(9, inputState);
}
~~~

