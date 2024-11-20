# Task 4 IIC SPI
## 学习内容
 - IIC
   - [什么是IIC] IIC（也叫I²C，Inter-Integrated Circuit）是一种由飞利浦公司（现为恩智浦半导体）在1982年开发的串行通信协议，用于集成电路之间的短距离通信。IIC协议在嵌入式系统中被广泛使用，例如在传感器、存储设备、显示模块和微控制器之间的通信。
     - 了解IIC总线
       - SDA 用于传输数据
       - CLK 用于同步通信
       - 学习IIC总线时序
   - [在STM32CUBEMX中配置iic接口(硬件iic)]在引脚配置界面 (Pinout & Configuration) 中，找到 I2C 外设。点击对应的 I2C 外设（如 I2C1 或I2C2），将其设置为 "I2C" 模式。STM32CubeMX 会自动分配 I2C 的 SCL 和 SDA 引脚。
   - [使用IIC控制ssd1306]连接对应引脚口后调用相应函数
 - SPI
   - [什么是SPI]SPI（Serial Peripheral Interface）是串行外围设备接口的缩写，由摩托罗拉公司开发，是一种常用的全双工高速通信协议，用于在微控制器和外围设备（如传感器、存储器、显示器等）之间进行通信。
     - 了解SPI总线
       - CLK:CLK 是 Clock（时钟）的缩写，在电子和计算机领域中，它指的是一个周期性的信号，用来为数字系统中的操作提供同步参考。时钟信号的作用是协调电路中各个部分的动作，使它们能够按照统一的节奏工作。
       - MOSI:MOSI 是 Master Out Slave In 的缩写，是在 SPI（Serial Peripheral Interface）通信协议中用于数据传输的信号线之一。它表示主设备发送数据到从设备的通道。
       - MISO:MISO 是 Master In Slave Out 的缩写，是在 SPI（Serial Peripheral Interface）通信协议中用于数据传输的信号线之一。它表示从设备发送数据到主设备的通道。
       - DC:在信号处理中，DC 表示信号的直流分量，也就是信号的平均值或零频率成分。
       - RW：在寄存器操作中，RW 表示该寄存器的字段或位是可读可写的。
       - RST：RST 是 Reset（复位） 的缩写，在电子、电气工程和计算机系统中，通常用于表示复位信号或复位功能。
       - 学习SPI总线时序
   - [在STM32CUBEMX中配置SPI接口(硬件spi)]3. 配置 I2C 参数
在 Configuration 选项卡中，找到 I2C 外设，点击它以打开参数配置窗口。
根据需要设置以下参数：
Timing Settings:
设置 Timing 参数，主要用于调整 I2C 的时钟频率。
通常使用 STM32CubeMX 提供的 Timing Calculator 工具。
I2C Mode:
选择 I2C 模式（主机或从机模式）。
Addressing Mode:
选择地址模式：7 位或 10 位。
Own Address 1:
如果是从机模式，设置自己的设备地址。
Dual Address Mode:
如果需要支持双地址模式，可以启用。
No Stretch Mode:
根据需要启用或禁用总线拉伸功能。
General Call Mode:
启用或禁用总线的广域调用功能。
4. 启用中断或 DMA（可选）
如果需要提高数据传输效率，可以启用 DMA 支持：
在 NVIC Settings 中启用 I2C 的中断。
或者在 DMA Settings 中添加对应的 DMA 通道（如 I2C1_RX、I2C1_TX）。
   - [使用SPI控制st7789]调用相应函数
 - 在oled屏幕上显示 姓名 50%
 - 在lcd屏幕上显示qq头像 50%


https://github.com/user-attachments/assets/c90b99cd-c35a-4c72-92fd-9bf8b79edec2



https://github.com/user-attachments/assets/8bccbac9-4f9a-4c39-b131-0068b6c599f5


 - 
