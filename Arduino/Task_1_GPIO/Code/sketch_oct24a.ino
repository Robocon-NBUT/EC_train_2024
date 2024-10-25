//让开发板上的 LED 以 1Hz 的频率闪烁。
void setup() {
  pinMode(13,OUTPUT);
}

void loop() {
  digitalWrite(13,HIGH);

  delay(500);

  digitalWrite(13,LOW);

  delay(500);
}
