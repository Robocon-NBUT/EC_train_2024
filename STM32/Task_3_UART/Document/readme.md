#  学习心得
##  异步串口
设备之间没有共用的时钟信号，发送方和接收方通过约定好的波特率来保持数据传输的同步。
## 同步串口
需要发送方和接收方共用一个时钟信号
## 如何使用HAL库配置stm32的串口
使用 HAL 库配置 STM32 串口步骤       
步骤一：开启串口和相关时钟    
首先，需要开启串口和对应的 GPIO 时钟    
步骤二：配置 GPIO 引脚   
使用各类HAL库   
步骤三：配置串口参数    
设置串口为 USARTx。   
设置波特率为    
设置数据字长为 8 位。   
设置停止位为 1 位。    
设置校验位为奇校验。    
设置为全双工模式（发送和接收）。    
完成串口初始化。    
步骤四：实现串口收发功能（以发送字符串为例）    
使用HAL库函数
## 如何使用stm32的usb虚拟串口
CubeMX 新建工程     
其中外部高速晶振源(HSE)     
USB_OTG_FS：选择 Device_Only;  设备模式(从机模式)       
USB_DEVICE：选择 CDC (VPC)       
usb系统时钟设为48mhz      
生成工程          
进行代码编写



https://github.com/user-attachments/assets/389fa15e-97f4-4232-9d27-7fd7ce7db421



https://github.com/user-attachments/assets/3309ec2e-bddf-4c93-9803-b9bfc8253009



