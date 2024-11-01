# 使用stm32cubemx进行灯板的控制

这是一个艰难的过程，不仅在端口的匹配上耗费了大量时间，也在pwm调光的理解上耗费了大量精力。

大量的代码编程以HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
		HAL_Delay(500);为基础。

同时在STM32cubemx端要进行设置，在pwm调光时要进行arr，ccr等计算，为我们提供了崭新的思路。