# What is arduino ？  this was wheelchair
 **Arduino** is an open-source platform made of hardware and software  
  its helps people easily create ***electronic projects***    
# AND？
   its  designed to help students quickly build different electronic devices and systems  
    but its performance is not sharp
## 请看vcr
## test1
点灯通过设定引脚输出电频来实现效果
每1000毫秒改变一次电频
~~~
void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}  
~~~
## test2
通过读取引脚状态同步状态实现开关
~~~
void setup() {
  pinMode(9, OUTPUT);
  pinMode(2, INPUT);
}

void loop() {
  int inputState = digitalRead(2);
  digitalWrite(9, inputState);          
}
~~~

## test3
通过设定引脚亮度来实现控制
并且·设定·该变量和每次改变间隔时间来实现呼吸灯效果
~~~
int ledPin = 9;
int brightness = 0;
int speed = 5;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  analogWrite(ledPin, brightness);
  brightness = brightness + speed;

  if (brightness <= 0 || brightness >= 255) {
    speed = -speed;
  }

  delay(30); 
}

~~~