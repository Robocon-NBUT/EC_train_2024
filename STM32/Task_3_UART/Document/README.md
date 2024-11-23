# 什么是串口
## 什么是虚拟串口 ？
串口是一种常见的通信协议，用于在计算机与外部设备之间传输数据。UART 是一种**异步**通信协议，意味着数据的发送与接收不依赖于共同的时钟信号，而是通过双方预设的波特率来同步数据传输。
而虚拟串口不需要相同波特率

## 总结
这次任务相对于上次任务来说简单了一点，但是第一次做的时候没看清任务要求，在用usart搞，偏离方向
## 同步串口和异步串口
**同步串口**在数据传输过程中会使用独立的时钟信号来同步发送和接收设备的数据传输。时钟信号可以在通信线中单独传输，也可以通过其他方式来同步，例如在特定协议中预设的同步时序。     

**异步串口**则没有独立的时钟信号，而是通过数据的起始位和停止位来确定数据帧的开始和结束。这种方式依赖接收端自行产生时钟，并通过比特位的预定义来保持与发送端的同步。

## 连接方式：
### USBCDC:
直接把usb与主板子连接（**注意时钟树中需要设置RC48为48！！！这是个坑！！不是48连不上**）
### 软件与硬件：
rx和tx对应这连接

## HAL设置：
### 软件串口：
设置输出引脚和定时器（注意启动中断），计算出波特率对应的psc和arr，同时要配置时钟树
### 硬件串口：
设置usart即可
### USBCDC:
设置USB-OTG-FS(全速模式），mode选择device only，然后在Middleware（后面还有一长串）里面找到USB_DEVICE,设置cdc模式

## 收获
stm32中串口有3种方式（目前学习的），有硬件串口，软件模拟串口，usbcdc串口

## HELLO WORLD
https://github.com/user-attachments/assets/d8a2f1d2-7a56-48d2-a27f-70af413867fa

## 复读机
https://github.com/user-attachments/assets/52202129-a90e-4daf-ac06-3ddfe004eba8
