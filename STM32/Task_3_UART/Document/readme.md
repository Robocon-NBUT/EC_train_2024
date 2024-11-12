什么是串口
Tx:发送数据
Rx:接收数据

异步串口：一个操作必须在另一个操作完成后开始执行。
同步串口：一个操作必须在另一个操作完成后开始执行。

如何使用hal库配置stm32的串口
找到USART1，将串口模式改为TTL模式——即Asynchronous(异步模式）
HAL_UART_Transmit(&huart1, transmitData, 2, HAL_MAX_DELAY);
HAL_UART_Receive(&huart1, receiveData, 2, 100);

串口怎么连接（提供的调试器有串口功能）
Tx与Rx相连

如何使用stm32的usb虚拟串口(usb-cdc vcp)
stm32中设置usb-otg-fs为Device_Only
再设置usb_Device
调整usb时钟为48mhz

https://github.com/user-attachments/assets/2b54bba4-50a1-4ab8-b89c-e97550c7b6a2



https://github.com/user-attachments/assets/32371f83-cf4b-4f26-9029-b502bed5afce

