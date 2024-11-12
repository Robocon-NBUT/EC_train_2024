# 串口发送与接受数据

## 异步串行与同步串行

异步串行：以字符为单位传送信息，时钟同步要求不高，以短距离、速率不高为主。
同步串行：以数据块为单位传送信息，要求有同步时钟，以大批量数据传输为主。

## 用hal库配置stm32串口

1.开启串口和相关 GPIO 的时钟

2.配置 GPIO 引脚

3.配置串口参数：波特率为115200，数据字长为8位，停止位为1位，奇校验。

## 相关新知

通过usbd_conf.c等底层文件进行编写，CDC_Transmit_FS的使用。
USB是轮询机制，间隔最小1ms，按包传输。要求我们输出字符串时要进行延时。

https://github.com/user-attachments/assets/41071302-f7d9-4550-8499-6f664e745e37



https://github.com/user-attachments/assets/f599671d-d779-49fa-be26-7491be4091de

