#include "st7789.h"

// 写入命令
static void ST7789_WriteCommand(uint8_t cmd)
{
    ST7789_Select();
    ST7789_DC_Clr();
    HAL_SPI_Transmit(&ST7789_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    ST7789_UnSelect();
}

// 写入数据
static void ST7789_WriteData(uint8_t *buff, size_t buff_size)
{
    ST7789_Select();
    ST7789_DC_Set();

    // 将数据分块发送，HAL不能一次性发送超过64K的数据
    while (buff_size > 0) {
        uint16_t chunk_size = buff_size > 65535 ? 65535 : buff_size;
        HAL_SPI_Transmit(&ST7789_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
        buff += chunk_size;
        buff_size -= chunk_size;
    }

    ST7789_UnSelect();
}       

// 写入一个小的数据
static void ST7789_WriteSmallData(uint8_t data)
{
    ST7789_Select();
    ST7789_DC_Set();
    HAL_SPI_Transmit(&ST7789_SPI_PORT, &data, sizeof(data), HAL_MAX_DELAY);
    ST7789_UnSelect();
}

// 设置屏幕旋转
void ST7789_SetRotation(uint8_t m)
{
    ST7789_WriteCommand(ST7789_MADCTL);
    switch (m) {
        case 0:
            ST7789_WriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
            break;
        case 1:
            ST7789_WriteSmallData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
            break;
        case 2:
            ST7789_WriteSmallData(ST7789_MADCTL_RGB);
            break;
        case 3:
            ST7789_WriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
            break;
        default:
            break;
    }
}

// 设置显示地址窗口
static void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ST7789_Select();
    uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
    uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

    // 设置列地址
    ST7789_WriteCommand(ST7789_CASET);
    {
        uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
        ST7789_WriteData(data, sizeof(data));
    }

    // 设置行地址
    ST7789_WriteCommand(ST7789_RASET);
    {
        uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
        ST7789_WriteData(data, sizeof(data));
    }

    // 写入到内存
    ST7789_WriteCommand(ST7789_RAMWR);
    ST7789_UnSelect();
}

// 初始化ST7789显示屏
void ST7789_Init(void)
{
    HAL_Delay(10);
    ST7789_RST_Clr();
    HAL_Delay(10);
    ST7789_RST_Set();
    HAL_Delay(20);

    ST7789_WriteCommand(ST7789_COLMOD);        // 设置颜色模式
    ST7789_WriteSmallData(ST7789_COLOR_MODE_16bit);
    ST7789_WriteCommand(0xB2);                 // Porch控制
    {
        uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
        ST7789_WriteData(data, sizeof(data));
    }
    ST7789_SetRotation(ST7789_ROTATION);      // 设置旋转

    // 内部LCD电压设置
    ST7789_WriteCommand(0XB7);                // Gate控制
    ST7789_WriteSmallData(0x35);
    ST7789_WriteCommand(0xBB);                // VCOM设置
    ST7789_WriteSmallData(0x19);
    ST7789_WriteCommand(0xC0);                // LCM控制
    ST7789_WriteSmallData(0x2C);
    ST7789_WriteCommand(0xC2);                // VDV和VRH命令启用
    ST7789_WriteSmallData(0x01);
    ST7789_WriteCommand(0xC3);                // VRH设置
    ST7789_WriteSmallData(0x12);
    ST7789_WriteCommand(0xC4);                // VDV设置
    ST7789_WriteSmallData(0x20);
    ST7789_WriteCommand(0xC6);                // 正常模式帧速率控制
    ST7789_WriteSmallData(0x0F);
    ST7789_WriteCommand(0xD0);                // 电源控制
    ST7789_WriteSmallData(0xA4);
    ST7789_WriteSmallData(0xA1);

    ST7789_WriteCommand(0xE0);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
        ST7789_WriteData(data, sizeof(data));
    }

    ST7789_WriteCommand(0xE1);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
        ST7789_WriteData(data, sizeof(data));
    }

    ST7789_WriteCommand(ST7789_INVON);        // 反转显示开启
    ST7789_WriteCommand(ST7789_SLPOUT);       // 唤醒模式
    ST7789_WriteCommand(ST7789_NORON);        // 正常显示开启
    ST7789_WriteCommand(ST7789_DISPON);       // 显示开启
    ST7789_Fill_Color(BLACK);  // 填充背景颜色
    HAL_Delay(50);
}

// 绘制图像
void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)
{
    if ((x >= ST7789_WIDTH) || (y >= ST7789_HEIGHT)) {
        return;
    }
    if ((x + w - 1) >= ST7789_WIDTH) {
        return;
    }
    if ((y + h - 1) >= ST7789_HEIGHT) {
        return;
    }

    // 选择显示屏并设置显示窗口
    ST7789_Select();
    ST7789_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    ST7789_WriteData((uint8_t *)data, w * h * sizeof(uint16_t));

    ST7789_UnSelect();
}

void ST7789_Fill_Color(uint16_t color)
{
    uint16_t i;
    ST7789_SetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
    ST7789_Select();

    for (i = 0; i < ST7789_WIDTH; i++) {
        for (uint16_t j = 0; j < ST7789_HEIGHT; j++) {
            uint8_t data[] = {color >> 8, color & 0xFF};
            ST7789_WriteData(data, sizeof(data));
        }
    }

    ST7789_UnSelect();
}

void ST7789_Test(void)
{
    LCD_ShowPicture(0,0,118,118,touxiang);
}
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint16_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	ST7789_SetAddressWindow(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			ST7789_WriteSmallData(pic[k*2]);
			ST7789_WriteSmallData(pic[k*2+1]);
			k++;
		}
	}			
}