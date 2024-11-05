# Task_2_GPIO_review
## 任务描述：
众所周知，数据是以二进制形式在计算机中储存及传输的。现提供一块已烧录程序的stm32开发板，其**PB15**引脚为数据输入引脚，需使用手中的stm32开发板通过控制引脚高低电平以**软件**形式实现**串口**通信，向目标开发板发送`hello world\0`(`\0`为字符串结束符)。若目标开发板接收到的数据正确，则其板载led电平会翻转(熄灭状态变点亮状态，点亮状态变熄灭状态)。

串口通信详解：https://blog.csdn.net/yueqiu693/article/details/120868246   
Stm32 IO模拟串口教程：https://blog.csdn.net/TonyIOT/article/details/82502953   

## 接收端串口参数
 - 波特率：500
 - 字长8位
 - 无奇偶校验位
 - 1位停止位
 - RX：PB15
 - 低位在前

## 伪代码
```c
void transmit(char c){
    set_data_low();//将数据线拉低，开始传输
    delay();//延时
    for(int i=0;i<8;i++){//八位
        if(c&0x01)//从最低位开始发送一个bit
            set_data_high();
        else
            set_data_low();
        c = c>>1;//右移一位，准备传输下一个bit
        delay();
    }
    set_data_high();//结束
    delay();
}
```

## 任务要求：
 - **禁用**硬件串口，若使用硬件串口**不得分**
 - 代码逻辑正确 60% （点不亮但是代码思路正确就给分）
 - 成功与给定开发板建立通信 40% （能点亮开发板上的灯）
 - 时限一周，提交方式与之前相同
