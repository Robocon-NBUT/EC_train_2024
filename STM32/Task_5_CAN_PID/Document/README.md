# Task 5 CAN PID

## CAN

> - 两根信号线，不靠时钟信号，用差分信号，抗干扰（通过两个线路（CAN_H和CAN_L）之间的电压差来传递信息。这种差分传输方式提供了良好的抗干扰性能，使得CAN总线适用于工业环境等有电磁干扰的场合。）
> - 异步
> - 数据帧（Data frame）
遥控帧（Remote frame）
错误帧（Error frame）
过载帧（Overload frame）
帧间空间（Inter-frame space）
> - 其中数据帧最重要，而仲裁场是向电机发送信号的关键

## PID
> 以下内容为个人理解大白话：    
> 
KP:
离目标越远，力越大砖越飞     
KI:
离目标近，但是还差一口气，最后再努力一下下     
KD:
避免反复横跳

## 收获

can和pid是常用的控制方式，pid的算法可以说很妙，但是一开始看公式有点难以理解。can的发送函数也较为难懂，数据较复杂，不过这也是can传输的稳定性好，抗干扰的秘密

## 视频
> 速度环：


https://github.com/user-attachments/assets/4d97029d-630e-4743-b168-3097c07d6d73


> 位置环：


https://github.com/user-attachments/assets/379c69ba-6f4b-495a-aa53-b107c58465cb



