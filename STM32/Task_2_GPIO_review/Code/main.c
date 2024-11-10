/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

#include "main.h"
#include "memorymap.h"
#include "tim.h"
#include "gpio.h"

// 全局变量，用于延时计数
volatile uint32_t delay_counter = 0;

// 定时器周期结束回调函数
// 当定时器TIM2的周期结束时会调用此函数
// 在此函数中，会递增延时计数器，并翻转指定LED引脚的电平状态
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        delay_counter++;
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
}

// 检查字符中 '1' 的个数是否为奇数的函数
// 遍历字符的每一位，统计 '1' 的个数，若为奇数则返回1，否则返回0
size_t isji(char c)
{
    int count = 0;
    for (size_t i = 0; i < 8; i++)
    {
        if (c & 0x01)
        {
            count++;
        }
        c = c >> 1;
    }
    return (count % 2 == 1)? 1 : 0;
}

// 自定义的延时函数，通过等待延时计数器达到指定值来实现延时
// 参数us为需要延时的微秒数
void my_delay(volatile uint32_t us)
{
    delay_counter = 0;
    while (delay_counter < us);
}

// 发送字符的函数
// 按照一定的协议将字符逐位发送出去，最后根据字符中 '1' 的奇偶性设置引脚状态
void transmit(char c)
{
    char origin_c = c;

    // 将发送引脚设置为低电平，准备发送数据
    HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_RESET);
    my_delay(1);

    // 逐位发送字符的8位数据
    for (size_t i = 0; i < 8; i++)
    {
        if (c & 0x01)
        {
            HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_RESET);
        }
        c = c >> 1;
        my_delay(1);
    }

    // 根据原始字符中 '1' 的奇偶性设置引脚状态
    if (isji(origin_c))
    {
        HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_RESET);
    }
    my_delay(1);

    // 发送完成后，将引脚设置为高电平
    HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_SET);
    my_delay(1);
}

// 系统时钟配置函数
// 此函数用于配置系统的各种时钟参数，包括振荡器、PLL等
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // 配置电源供应相关设置
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    // 配置主内部调节器输出电压
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    // 初始化RCC振荡器参数
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 2;
    RCC_OscInitStruct.PLL.PLLN = 32;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;

    // 配置振荡器，如果配置失败则调用错误处理函数
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
    {
        Error_Handler();
    }

    // 初始化CPU、AHB和APB总线时钟参数
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                  | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    // 配置时钟，如果配置失败则调用错误处理函数
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4)!= HAL_OK)
    {
        Error_Handler();
    }
}

// MPU配置函数
// 用于配置内存保护单元（MPU）的相关参数
void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    // 先禁用MPU
    HAL_MPU_Disable();

    // 初始化并配置MPU的区域和要保护的内存相关参数
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x0;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

    // 配置MPU区域
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    // 启用MPU，并设置为特权模式默认状态
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

// 错误处理函数
// 在发生错误时被调用，此处示例中是进入死循环并禁用中断
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

// 断言失败处理函数（仅在USE_FULL_ASSERT被定义时有效）
// 可用于报告断言参数错误发生的文件名和行号
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

int main(void)
{
    // MPU配置
    MPU_Config();

    // 初始化HAL库
    HAL_Init();

    // 配置系统时钟
    SystemClock_Config();

    // 初始化配置好的外设
    MX_GPIO_Init();
    MX_TIM2_Init();

    // 启动定时器TIM2的中断功能
    HAL_TIM_Base_Start_IT(&htim2);

    // 定义要发送的字符串
    char str[13] = "hello world\0";

    // 无限循环
    while (1)
    {
        // 逐个发送字符串中的字符
        for (size_t i = 0; i < 13; i++)
        {
            transmit(str[i]);
        }

        // 发送完一轮字符串后，延时1000毫秒（1秒）
        HAL_Delay(1000);
    }

    return 0;
}