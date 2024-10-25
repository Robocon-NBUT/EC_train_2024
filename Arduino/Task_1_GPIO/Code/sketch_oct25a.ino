void setup()  
{  
  pinMode(4, INPUT); // 将 4 号引脚设置为输入  
  pinMode(9, OUTPUT); // 将 9 号引脚设置为输出  
}  
void loop()  
{  
  int open = digitalRead(4); // 读取 4 号引脚的输入值  
  digitalWrite(9, open); // 将 4 号引脚的输入值输出到 9 号引脚  
}