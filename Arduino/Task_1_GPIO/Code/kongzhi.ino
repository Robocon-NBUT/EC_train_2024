void loop()  
{  
for(int i=0;i<=255;i++)  
  {  
    analogWrite(9,i);  //通过循环设置模拟量的升高输入使灯变亮  
    delay(5);  
  }  
   for(int i=255;i>=0;i--)  
  {  
    analogWrite(9,i);   //通过循环设置模拟量的降低输入使灯变暗  
    delay(5);  
  }   
}