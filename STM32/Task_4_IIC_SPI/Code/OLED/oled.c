#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

#define OLED_ADD 0x78

void Write_IIC_Command(unsigned char i2c_cmd)
{
    uint8_t *cmd;
    cmd = &i2c_cmd;
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0x00, I2C_MEMADD_SIZE_8BIT, cmd, 1, 100);
}

void Write_IIC_Data(unsigned char i2c_data)
{
    uint8_t *data;
    data = &i2c_data;
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0x40, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

void OLED_WR_Byte(unsigned char dat, unsigned char cmd)
{
    if (cmd)
    {
        Write_IIC_Data(dat);
    }
    else
    {
        Write_IIC_Command(dat);
    }
}

// 坐标设置

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
// 开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
// 关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}
// 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);     // 设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);     // 设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    } // 更新显示
}
void OLED_On(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);     // 设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);     // 设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(1, OLED_DATA);
    } // 更新显示
}

// 显示汉字
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for (x = x0; x < x1; x++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}

// 初始化SSD1306
void OLED_Init(void)
{
    OLED_WR_Byte(0xAE, OLED_CMD); //--display off
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD); //--128
    OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
    OLED_WR_Byte(0x00, OLED_CMD); //

    OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
    OLED_WR_Byte(0x80, OLED_CMD); //

    OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD); //

    OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD); //

    OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD); //

    OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); //

    OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD); //

    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}
