# I2C SPI

<br/>

## I2C — 同步的多主机通信总线

### 主要特点

1·多主机支持： 可多主机和多从设备连接到同一总线

2·双向通信 ：主设备和从设备可双向数据传输

3·简单的物理接口： 只要两根线 一根 SDA数据线，一根SCL时钟线，使I2C的实用性大大增加

      起始位：           SCL         ————————————

                                        高电压                         

                              SDA      ————————  下降沿

                                                ———————

     停止位 ：            SCL         上升   ————————

                                —————— 
                                             高电压

                               SDA       上升沿   ———————

                                    ————   

     I2C用高低变化的电压来传输数据，中间将要传输的数据以0101的形式发送

<br/>

## SSD1306

### OLED

使用I2C来控制OLED屏幕，来输出你想要输出的文字

OLED有四个接口，VCC —— 电源3v3 

                GND —— 接地

                SCL —— I2C的串行时钟线

                SDA —— I2C的串行数据线

代码OLED_CLear（）；

      OLED_Chinese（x , y , z）；       x , y 来控制字在屏幕中的位置， z 从0 开始，有几个字就把此行代码复制几个

      OLED_Chinese（x , y , 0）；

      OLED_Chinese（x , y , 1）；     以此类推

<br/>

在学习过程中学会了如何添加驱动代码，直接在文件中添加text.c文件，已经通过main.c来打开文件夹来将text.h文件导入，让代码正常运行

<br/>

## SPI

### 串行外围设备接口

基本信号线：

1 · MOSI（Master Out Slave In）：主设备数据输出，从设备数据输入
2 · MISO（Master In Slave Out）：主设备数据输入，从设备数据输出
3 · SCLK（Serial Clock）：时钟信号，由主设备产生
4 · CS（Chip Select）：从设备片选信号，由主设备控制，低电平有效
5 · DC（Direct Current）：直流电
6 · RW (Read/Write) : 读取和修改文件
7 · RST(Reset) : 是复位信号，用于将设备或系统恢复到初始状态

<br/>

### 用cubemx配置SPI1

改成8—Bits       传输速度改为HIGH        要额外设置三个GPIO引脚output来连接剩下的串口

还是要把驱动文件放入CORE文件夹中，再对图片进行16位色彩的取模，正常输出图像