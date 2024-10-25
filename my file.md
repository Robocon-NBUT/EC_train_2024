
# 我是xxx
第一是

~~~const int ledPin = LED_BUILTIN; // 使用内置LED引脚

void setup() {
    pinMode(ledPin, OUTPUT); // 设置LED引脚为输出模式
}

void loop() {
    digitalWrite(ledPin, HIGH); // 打开LED
    delay(500);                 // 等待500毫秒
    digitalWrite(ledPin, LOW);  // 关闭LED
    delay(500);                 // 等待500毫秒
}
~~~
## 第二