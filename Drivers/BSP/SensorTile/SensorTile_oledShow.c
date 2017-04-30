#include "SensorTile_oledShow.h"
#include "ssd1306.h"
#include <string.h>
#include "targetfeatures.h"
#include "main.h"

static void showDeviceName(void)
{
	char buf[20] = {0};
	
	ssd1306_ClearScreen();
	HAL_Delay(10);
	
	ssd1306_SetCursor(16,18);
	ssd1306_WriteChineseString(0, 6, FontChinese_16x16, White);
	ssd1306_SetCursor(36,45);
	sprintf(buf, "%s", __TIME__);
	ssd1306_WriteString(buf, Font_7x10, White);
	ssd1306_UpdateScreen();
	HAL_Delay(10);
}

static void showLoading(void)
{
	ssd1306_ClearScreen();
	HAL_Delay(10);
	ssd1306_SetCursor(16,24);
	ssd1306_WriteChineseString(6, 4, FontChinese_16x16, White);
	ssd1306_WriteString("...", Font_11x18, White);
	ssd1306_UpdateScreen();
	HAL_Delay(10);
}

static void showSensorData(void)
{
	ssd1306_ClearScreen();
	HAL_Delay(10);
	
	ssd1306_SetCursor(0,0);
	ssd1306_WriteChineseString(12, 2, FontChinese_16x16, White); // 温度
	ssd1306_WriteString(":", Font_16x16, White);
	
	ssd1306_SetCursor(0,23);
	ssd1306_WriteChineseString(14, 2, FontChinese_16x16, White); // 湿度
	ssd1306_WriteString(":", Font_16x16, White);
	
	ssd1306_SetCursor(0,47);
	ssd1306_WriteChineseString(10, 2, FontChinese_16x16, White); // 压强
	ssd1306_WriteString(":", Font_16x16, White);
	
	ssd1306_SetCursor(111,0);
	ssd1306_WriteChar(127, Font_16x16, White); // 温度单位 
	
	ssd1306_SetCursor(111,23);
	ssd1306_WriteString("%", Font_16x16, White); // 湿度单位
	
	ssd1306_SetCursor(112,50);
	ssd1306_WriteString("mb", Font_7x10, White); // 压强单位
	
	ssd1306_UpdateScreen();
	HAL_Delay(10);
}


static void showFallDetect(void)
{
	ssd1306_ClearScreen();
	HAL_Delay(10);
	ssd1306_SetCursor(20,24);
	ssd1306_WriteChineseString(16, 6, FontChinese_16x16, White);
	ssd1306_UpdateScreen();
	HAL_Delay(10);
}

void SensorTile_oledShow(Page info)
{
	if(info == DeviceName)
		showDeviceName();
	else if(info == Loading)
		showLoading();
	else if(info == SensorData)
		showSensorData();
	else if(info == FallDetect)
		showFallDetect();
}



