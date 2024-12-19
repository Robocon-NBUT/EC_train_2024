# Task 5 CAN PID
## 学习内容
 - CAN
   - [什么是CAN总线]
      - CAN（Controller Area Network）总线是一种串行通信协议，主要用于在汽车电子系统、工业自动化等领域实现设备之间的数据通信。
   - 了解CAN总线
     - TTL电平
     - 差分信号
      -  当 CAN - H 的电压高于 CAN - L 时，通常表示逻辑 “0”；当 CAN - H 的电压低于 CAN - L 时，表示逻辑 “1”。
   - [在STM32中配置CAN总线]
   - 使用STM32控制m2006电机动起来
   - 

https://github.com/user-attachments/assets/5df33629-65de-4ce4-89b8-c1e151875246


 - PID
   - [学习PID算法]
   - KP:KP 的值越大，控制器对当前误差的反应就越强烈。
   - KI:KI 是积分系数，它与误差的积分有关。
   - KD:微分项的作用是预测系统误差的变化趋势，从而提前进行调整。
 - 使用PID算法实现m2006电机的位置控制


https://github.com/user-attachments/assets/0be7dd0f-da50-4ec1-996a-876dabbe1dbe

