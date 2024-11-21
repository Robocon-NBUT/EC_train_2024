#
<center><font face ="楷体" size=300>IIC和SPL</font></center>

<center><font  face="楷体" size=5> 谭尧瑞</font></center>
<font face="楷体" size=5>



# 学习笔记：IIC与SPI接口

## 引言
在嵌入式系统开发中，IIC（Inter-Integrated Circuit）和SPI（Serial Peripheral Interface）是两种常用的串行通信协议。本学习笔记旨在记录我对这两种协议的理解和学习心得。

## IIC学习心得

### 什么是IIC
IIC是一种同步的、多主机、多从机的串行通信协议，广泛应用于微控制器和外围设备之间的通信。

### IIC总线
- **SDA** (Serial Data Line)：数据线，用于传输数据。
- **SCL** (Serial Clock Line)：时钟线，用于同步数据传输。

### IIC总线时序
学习了IIC的时序图，理解了起始条件、数据传输、停止条件等关键时序。

### STM32CUBEMX配置IIC
在STM32CUBEMX中配置IIC接口，通过图形化界面简化了硬件配置过程。

### 使用IIC控制SSD1306
通过IIC接口控制SSD1306 OLED显示屏，实现了基本的显示功能。

## SPI学习心得

### 什么是SPI
SPI是一种高速的、全双工、同步的通信总线，通常用于微控制器与各种外围设备之间的通信。

### SPI总线
- **CLK** (Clock)：时钟信号，用于同步数据传输。
- **MOSI** (Master Out Slave In)：主设备输出，从设备输入。
- **MISO** (Master In Slave Out)：主设备输入，从设备输出。
- **DC** (Data/Command)：数据/命令选择。
- **RW** (Read/Write)：读/写选择。
- **RST** (Reset)：复位信号。

### SPI总线时序
学习了SPI的时序图，包括片选信号、数据传输等。

### STM32CUBEMX配置SPI
在STM32CUBEMX中配置SPI接口，同样通过图形化界面简化了硬件配置。

### 使用SPI控制ST7789
通过SPI接口控制ST7789 TFT显示屏，实现了更复杂的显示功能。

## 任务要求完成情况
- 在OLED屏幕上显示姓名，完成了
- 在TFT屏幕上显示QQ头像，完成了

## 结论
通过本次学习，我对IIC和SPI两种通信协议有了更深入的理解。在实际应用中，这两种协议各有优势，适用于不同的场景。IIC因其简单性和灵活性，在短距离通信中非常实用；而SPI则因其高速和全双工特性，在需要快速数据传输的应用中更为合适。

## 未来计划
在未来的项目中，我计划进一步探索这两种协议的高级特性，并尝试在更复杂的系统中应用它们。