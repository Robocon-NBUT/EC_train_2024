const int ledPin = 9; // LED 连接的引脚
int brightness = 0;     // LED 亮度
int fadeAmount = 5;     // 每次变化的亮度值

void setup() {
  pinMode(ledPin, OUTPUT); // 设置 LED 引脚为输出模式
}

void loop() {
  // 设置 LED 亮度
  analogWrite(ledPin, brightness);
  
  // 改变亮度值
  brightness += fadeAmount;

  // 反转亮度变化方向
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount; // 改变变化方向
  }

  // 等待一段时间，以产生呼吸效果
  delay(30);
}