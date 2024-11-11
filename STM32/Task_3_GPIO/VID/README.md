# 什么是串口
* 串行接口简称串口，也称串行通信接口或串行通讯接口（通常指COM接口），是采用串行通信方式的扩展接口。串行接口 （Serial Interface）是指数据一位一位地顺序传送。

* 异步串口:串行接口简称串口，也称串行通信接口或串行通讯接口（通常指COM接口），是采用串行通信方式的扩展接口。串行接口 （Serial Interface）是指数据一位一位地顺序传送。

* 同步串口:串行接口简称串口，也称串行通信接口或串行通讯接口（通常指COM接口），是采用串行通信方式的扩展接口。串行接口 （Serial Interface）是指数据一位一位地顺序传送。
# 如何使用hal库配置stm32的串口
* 选择Connectivity → USB_OTG_FS → Mode → Device_Only。
* 选择Middleware → USB_DEVICE → Class For FS IP → Communication Device Class（Virtual Port Com）。
# 串口怎么连接（提供的调试器有串口功能）
* 主板RT连接收端RX，主板RX连发出端RT
# 如何使用stm32的usb虚拟串口(usb-cdc vcp)
* 下载驱动后连接主板上的typ_c
