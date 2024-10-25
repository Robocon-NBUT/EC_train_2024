# RUNOOB Markdown Test
# 电控作业    俞浩然
__________________________________________

## 让LED以1HZ频率闪烁
```js


void setup() {
    pinMode(9,OUTPUT);
}

void loop() {
  
  digitalWrite(9,HIGH);
  delay(1000);
  digitalWrite(9,LOW); 
  delay(1000);

}


```


______

## 使一个数字引脚与另一个数字引脚同步 LED 状态。呼吸灯

```js
void setup() {  
  pinMode(13,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(1,OUTPUT);
}

void loop() {
  
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);

  for (int brightness =0 ; brightness <=255 ; brightness++ ){
    analogWrite(9,brightness);
    digitalWrite(1,LOW);
    delay(10);
  }
  
  for (int brightness =255 ; brightness >=0 ; brightness-- ){
    analogWrite(9,brightness);   
    delay(10); 
  }
  digitalWrite(1,HIGH);
}
```

