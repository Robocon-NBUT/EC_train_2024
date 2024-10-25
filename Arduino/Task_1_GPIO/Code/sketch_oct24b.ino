int led = 9;  // LED引脚
int bri = 0;  // LED亮度
int chg_amt = 5;  // 亮度变化量

void setup() 
{
  pinMode(led, OUTPUT);  // 设置LED引脚为输出模式
}

void loop() 
{
  analogWrite(led, bri);  // 设置LED的亮度
  bri = bri + chg_amt;         // 改变亮度值

  // 当亮度达到最大或最小时，反转亮度变化方向
  if (bri <= 0 || bri >= 255) 
  {
    chg_amt = -chg_amt;
  }
  //if (brightness <= 0)
  //{
  //  delay(1000);
  //}

  delay(30);  // 控制呼吸灯的速度
}
