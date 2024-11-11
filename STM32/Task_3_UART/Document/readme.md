# <mark><font face="华文彩云" color=red>串口通信学习心得


## &emsp;&emsp; <font face="华文黑体" color=#7D00FF>🍤 理解串口通信原理

&emsp;&emsp;这是基础，清楚串口是按串行方式传数据的接口。STM32串口的寄存器配置很关键，还有波特率，它影响数据传输速度，设置错了通信就会出问题。
## &emsp;&emsp; <font face="华文黑体" color=#7D00FF>🍤 串口配置重点
&emsp;&emsp;要使能串口时钟，这是工作前提。同时设置数据位长度、停止位、奇偶校验位等参数，严格按手册设值，初始化结构体可让配置更清晰。
## &emsp;&emsp; <font face="华文黑体" color=#7D00FF>🍤 问题与解决
&emsp;&emsp;像波特率错致数据乱码，重新核对计算和设置值。接收数据丢失就增大缓冲区、优化接收中断程序。这让我明白细节和调试的重要性。

<table><tr><td bgcolor=#E6D5B8><font size=3>学到了什么</td></tr></table>

- [x] **轮询模式串口通信**
- [x] **IT模式串口通信**
- [x] **DMA模式串口通信**	
- [x] **cdc模拟串口**
- [x] **利用串口空闲中断收发不定长数据**     




https://github.com/user-attachments/assets/c81ff693-6515-473d-ba70-e1c2a31269be






https://github.com/user-attachments/assets/3454873a-7f17-4469-bb3c-9e828d61d66f



https://github.com/user-attachments/assets/64fc54a6-b40b-4097-972b-28748de4057b


