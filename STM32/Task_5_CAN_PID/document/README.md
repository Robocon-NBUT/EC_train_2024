CAN通讯；
CAN总线由两根信号线，即图2和图3中的CANH和CANL，没有时钟同步信号。所以CAN是一种异步通信方式，与UART的异步通信方式类似，而SPI、I2C是以时钟信号同步的同步通信方式。
两根信号线的电压差CANH-CANL表示CAN总线的电平，与传输的逻辑信号1或0对应。对应于逻辑1的称为隐性（Recessive）电平，对应于逻辑0成为显性（Dominant）电平。


PID：
计算PID的第一步就是计算误差（Error）：误差=目标值-反馈值。

比例环节P(Proportion)
积分环节I(Integral)
微分环节D(Differential)

心得：
这次作业有难度，自己一个人很难完成，而且没什么头绪，找了许多资料还是不能使电机转起来，还是在其他伙伴和学长的帮助下才成功了

https://github.com/user-attachments/assets/928e6f74-57a9-475d-912a-3879f4a1ed88

