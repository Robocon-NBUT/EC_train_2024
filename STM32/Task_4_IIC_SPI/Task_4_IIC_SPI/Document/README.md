#
<center><font face ="楷体" size=300>IIC        SPI</font></center>

<center><font  face="楷体" size=5>黄广松</font></center>
<font face="楷体" size=5>一：IIC 概述

IIC 是一种简单的双向二线制同步串行总线，常用于连接微控制器与外围设备，两根线分别为串行数据线（SDA）和串行时钟线（SCL），能减少系统连线。

<font face="楷体" size=5>二：IIC 总线原理
1.SDA（Serial Data Line）：双向传输数据，在 SCL 时钟同步下，主从设备借此线互传数据。
2.SCL（Serial Clock Line）：提供时钟信号，其上升沿或下降沿确定数据何时有效及读取写入时机，确保数据传输同步。
<font face="楷体" size=5>三：SPI 概述
 SPI 是高速、全双工、同步通信总线，适用于短距离通信，通常至少需四根线，包括时钟线（CLK）、主出从入线（MOSI）、主入从出线（MISO），还可能有控制信号线。
<font face="楷体" size=5>四：SPI 总线原理
1.CLK（Clock）：提供时钟同步数据传输，可设置时钟极性（CPOL）与相位（CPHA），灵活控制数据采样时刻。
2.MOSI（Master Out Slave In）：主设备在 CLK 同步下经此线向从设备发数据。
3.MISO（Master In Slave Out）：从设备在 CLK 同步下借此线向主设备传数据。
4.DC（Data/Command）、RW（Read/Write）、RST（Reset）（部分设备）：
  DC：区分传输数据或命令，如显示设备中控制数据与命令传输。
  RW：指示数据传输方向，高电平读、低电平写（部分设备）。
RST：复位从设备，低电平有效使从设备恢复初始态。




















