void setup() {
    pinMode(9, OUTPUT); // 设置LED引脚为输出模式
}

void loop() {
    digitalWrite(9, HIGH); // 打开LED
    delay(1000);                 // 等待1000毫秒
    digitalWrite(9, LOW);  // 关闭LED
    delay(1000);                 // 等待1000毫秒
}