const int ledPin = 9;  // LED 连接的引脚
const int inputPin = 2; // 输入引脚

void setup() {
  pinMode(ledPin, OUTPUT);   // 设置 LED 引脚为输出
  pinMode(inputPin, INPUT);  // 设置输入引脚为输入
}

void loop() {
  // 读取输入引脚的状态
  int inputState = digitalRead(inputPin);
  
  // 根据输入状态设置 LED 状态
  digitalWrite(ledPin, inputState);
  
  // 可以添加小延迟以避免快速闪烁
  delay(100);
}
