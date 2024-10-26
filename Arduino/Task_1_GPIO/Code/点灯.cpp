int ontime=1000;
int delaytime=1000;//设置两个量
void setup() {
 pinMode(13,OUTPUT);//向13号引脚通电

}

void loop() {

  digitalWrite(13,HIGH);//向13号引脚通高电压
  delay(ontime);//暂停1s
  digitalWrite(13,LOW);//13号引脚通低电压
  delay(delaytime);
}
