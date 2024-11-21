# Task 4 IIC SPI

> ## IIC

 与之前学的USART和USBCDC类似，IIC也是一种硬件通讯方式

### SDA——串行数据线

- 用于传输数据位
- 双向，可以接收也可以发送
- 可分为单工，半双工，双工
- 主设备和接收设备都要控制

### SCL——串行时钟线

- 同步时钟脉冲
- 作为传输开始/结束标志

> ## SPI

SPI是一种串行外围设备接口

- 高速的
- 全双工

- CLK：串行时钟信号
- MISO：主机到从机的信号线
- MOSI：从机到主机的信号线
- DC：显示屏传输所需
- CS：芯片选择信号，拉低表示开始（与串口通讯一样）
- RW：表示读还是写的信号，高电平写，低电平读
- RST：复位引脚

> ## 点亮过程：

OLED：

1. 设置iic
2. 下载驱动库，.c放src,.h放inc（这个可以随意）
3. 检查你的keil5项目是否设置好inc的路径（要读取到.h文件）
4. 取模，放入font.c
5. 放完记得在font.h里声明
6. 在main.c中调用函数

LCD:

1. 启动spi
2. 设置时钟树，不要太高，在40mhz以内
3. 下载驱动库，.c放src,.h放inc（这个可以随意）
4. 检查你的keil5项目是否设置好inc的路径（要读取到.h文件）
5. 取模，高位在前，输出灰度256（如果你的图片特别复杂的话，太高了无法输出，不过256的图片会很黄）
6. 在fonts.h里面声明
7. 在main.c中调用函数

> ## 视频-oled-name：


https://github.com/user-attachments/assets/5b94fc71-44f0-4e0c-baa4-3e57c86ca514

> ## 照片-lcd-photo：

![微信图片_20241121201852](https://github.com/user-attachments/assets/ae30fb60-e239-463d-a5cc-a2e48d60e3ad)



