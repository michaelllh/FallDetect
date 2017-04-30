
/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>

#include "stm32l4xx_hal.h"
#include "sensortile.h"
#include "ssd1306.h"

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

static SSD1306_t SSD1306;

static I2C_HandleTypeDef hi2c3;

// IIC开始 
void IIC_Start(void)  
{ 
  SDA_OUT();    
  IIC_SDA_HIGH;           
  IIC_SCL_HIGH;  
  HAL_Delay(4);  
  IIC_SDA_LOW; //START:when CLK is high,DATA change form high to low   
  HAL_Delay(4);  
  IIC_SCL_LOW;//  
}      
// IIC停止
void IIC_Stop(void) 
{ 
  SDA_OUT(); 
  IIC_SCL_LOW; 
  IIC_SDA_LOW;//STOP:when CLK is high DATA change form low to high  
  HAL_Delay(4);  
  IIC_SCL_HIGH;   
  IIC_SDA_HIGH;
  HAL_Delay(4);                     
} 

uint8_t IIC_Wait_Ack(void) 
{ 
	uint8_t ucErrTime=0;  
	SDA_IN();      
	IIC_SDA_HIGH;HAL_Delay(1);      
	IIC_SCL_HIGH;HAL_Delay(1);      
	while(READ_SDA) 
	{ 
		ucErrTime++;  
		if(ucErrTime>250)  
		{ 
			IIC_Stop();  
			return 1;  
		}
	} 
	IIC_SCL_LOW;        
	return 0;     
}


void IIC_Ack(void)  
{ 
	IIC_SCL_LOW;  
	SDA_OUT(); 
	IIC_SDA_LOW;  
	HAL_Delay(2);  
	IIC_SCL_HIGH;  
	HAL_Delay(2);  
	IIC_SCL_LOW;  
} 

void IIC_NAck(void)  
{ 
	IIC_SCL_LOW;  
	SDA_OUT(); 
	IIC_SDA_HIGH;  
	HAL_Delay(2);  
	IIC_SCL_HIGH;  
	HAL_Delay(2);  
	IIC_SCL_LOW;  
} 

void IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;    
	SDA_OUT();        
	IIC_SCL_LOW; // 拉低时钟线开始传输
	for(t=0;t<8;t++)  
	{               
		IIC_SDA_WRITE((txd&0x80)>>7);  
		txd<<=1;    
		HAL_Delay(2);  
		IIC_SCL_HIGH;   
		HAL_Delay(2);  
		IIC_SCL_LOW;  
		HAL_Delay(2);
	}		
}

uint8_t IIC_Read_Byte(unsigned char ack) 
{ 
	unsigned char i,receive=0;  
	SDA_IN();
	for(i=0;i<8;i++ )  
	{ 
		IIC_SCL_LOW;  
		HAL_Delay(2);  
		IIC_SCL_HIGH;  
		receive<<=1; 
		if(READ_SDA)receive++;     
		HAL_Delay(1);  
	}             
	if (!ack) 
		IIC_NAck();
	else  
		IIC_Ack();    
	return receive;  
} 

//
//	Een byte sturen naar het commando register
//	Kan niet gebruikt worden buiten deze file
//
static void ssd1306_WriteCommand(uint8_t command)
{
	HAL_I2C_Mem_Write(&hi2c3,SSD1306_I2C_ADDR,0x00,1,&command,1,10);
//	HAL_Delay(2);
//	IIC_Start();
//	IIC_Send_Byte(SSD1306_I2C_ADDR);
//	IIC_Wait_Ack();
//	IIC_Send_Byte(command);
//	IIC_Wait_Ack(); 
//	IIC_Stop();
//	HAL_Delay(2);
}


void ssd1306_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
	
	RCC_PeriphCLKInitStruct.PeriphClockSelection = SENSORTILE_I2C_ONBOARD_SENSORS_RCC_PERIPHCLK;
	RCC_PeriphCLKInitStruct.I2c3ClockSelection = SENSORTILE_I2C_ONBOARD_SENSORS_I2CCLKSOURCE;
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
    /* Peripheral clock enable */
	__HAL_RCC_I2C3_CLK_ENABLE();
    __I2C3_CLK_ENABLE();


//	hi2c3.Instance = I2C3;
//	hi2c3.Init.Timing = 0x00900000;                                     //400kHz,10ns,10ns,slave
//	hi2c3.Init.OwnAddress1 = 0x08;                                      //电调地址
//	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;                // 地址为7位
//	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
//	hi2c3.Init.OwnAddress2 = 0;
//	hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
//	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	
	hi2c3.Instance = I2C3;
	hi2c3.Init.Timing = 0x00000E14;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hi2c3);
  
	//__HAL_I2C_ENABLE(&hi2c3);


//	GPIO_InitTypeDef GPIO_InitStruct;
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//	IIC_SCL_HIGH;
//	IIC_SDA_HIGH;
	
	// Even wachten zodat het scherm zeker opgestart is
	HAL_Delay(100);
	
	/* Init LCD */
	ssd1306_WriteCommand(0xAE); //display off
	ssd1306_WriteCommand(0x20); //Set Memory Addressing Mode   
	ssd1306_WriteCommand(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	ssd1306_WriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_WriteCommand(0xC8); //Set COM Output Scan Direction
	ssd1306_WriteCommand(0x00); //---set low column address
	ssd1306_WriteCommand(0x10); //---set high column address
	ssd1306_WriteCommand(0x40); //--set start line address
	ssd1306_WriteCommand(0x81); //--set contrast control register
	ssd1306_WriteCommand(0xFF);
	ssd1306_WriteCommand(0xA1); //--set segment re-map 0 to 127
	ssd1306_WriteCommand(0xA6); //--set normal display
	ssd1306_WriteCommand(0xA8); //--set multiplex ratio(1 to 64)
	ssd1306_WriteCommand(0x3F); //
	ssd1306_WriteCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	ssd1306_WriteCommand(0xD3); //-set display offset
	ssd1306_WriteCommand(0x00); //-not offset
	ssd1306_WriteCommand(0xD5); //--set display clock divide ratio/oscillator frequency
	ssd1306_WriteCommand(0xF0); //--set divide ratio
	ssd1306_WriteCommand(0xD9); //--set pre-charge period
	ssd1306_WriteCommand(0x22); //
	ssd1306_WriteCommand(0xDA); //--set com pins hardware configuration
	ssd1306_WriteCommand(0x12);
	ssd1306_WriteCommand(0xDB); //--set vcomh
	ssd1306_WriteCommand(0x20); //0x20,0.77xVcc
	ssd1306_WriteCommand(0x8D); //--set DC-DC enable
	ssd1306_WriteCommand(0x14); //
	ssd1306_WriteCommand(0xAF); //--turn on SSD1306 panel
	
	/* Clearen scherm */
	ssd1306_Fill(Black);
	
	/* Update screen */
	ssd1306_UpdateScreen();
	
	/* Set default values */
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	/* Initialized OK */
	SSD1306.Initialized = 1;
	
	ssd1306_ClearScreen();
}

//
//	We zetten de hele buffer op een bepaalde kleur
// 	color 	=> de kleur waarin alles moet
//
void ssd1306_Fill(SSD1306_COLOR color) 
{
	/* Set memory */
	uint32_t i;

	for(i = 0; i < sizeof(SSD1306_Buffer); i++)
	{
		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}

//
//	Alle weizigingen in de buffer naar het scherm sturen
//
void ssd1306_UpdateScreen(void) 
{
	uint8_t i;
	
	for (i = 0; i < 8; i++) {
		ssd1306_WriteCommand(0xB0 + i);
		ssd1306_WriteCommand(0x00);
		ssd1306_WriteCommand(0x10);

		// We schrijven alles map per map weg
		HAL_I2C_Mem_Write(&hi2c3,SSD1306_I2C_ADDR,0x40,1,&SSD1306_Buffer[SSD1306_WIDTH * i],SSD1306_WIDTH,100);
	}
}

//
//	在显示屏上画点
//	X => X 坐标轴
//	Y => Y 坐标轴
//	color => kleur die pixel moet krijgen
//
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
	// 判断是否越界
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
		return;
	}
	
	// 黑白定义是否进行一个转换
	if (SSD1306.Inverted) 
	{
		color = (SSD1306_COLOR)!color;
	}
	
	// 更新buffer内容
	if (color == White)
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} 
	else 
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

//
//	这个函数一次更新一个字符到Buffer中
//	ch 		=> char 要显示的字符
//	Font 	=> Font 的名字
//	color 	=> Black or White
//
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
	uint32_t i, b, j;
	
	// 判断当前坐标位置能够画下字符
	if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
	{
		return 0;
	}
	
	// 更新buffer
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000) 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
			} 
			else 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
			}
		}
	}
	
	// 当前绘制光标往左移
	SSD1306.CurrentX += Font.FontWidth;
	
	// We geven het geschreven char terug voor validatie
	return ch;
}

//
//	OLED显示字符串的
// 	str => 要显示的字符串
//	Font => Het font dat gebruikt moet worden
//	color => Black or White
//
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
	while (*str) 
	{
		if (ssd1306_WriteChar(*str, Font, color) != *str)
		{
			return *str;
		}
		
		str++;
	}
	
	// 结束执行的话应该返回的是0
	return *str;
}

//
//	设置开始显示位置的光标坐标
//
void ssd1306_SetCursor(uint8_t x, uint8_t y) 
{
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

/*
 * @brief 显示中文汉字函数 ，注意使用这个函数先要对汉字进行编码并填写到FontChinese16x16，编码方式查看fonts.c说明
 * @param index 在fonts.c中的汉字编码数组中FontChinese16x16汉字的下标，从0开始
 * @param count 要显示FontChinese16x16中的几个汉字
 */
void ssd1306_WriteChineseString(uint8_t index, uint8_t count, FontDef Font, SSD1306_COLOR color)
{
	uint8_t i = 0, b, j, k;
	for(k=0; k < count; k++)
	{	
		uint32_t i, b, j;
		
		// 判断当前坐标位置能够画下字符
		if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
			SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
		{
			return;
		}
		
		// 更新buffer
		for (i = 0; i < Font.FontHeight; i++)
		{
			b = Font.data[( index +k)* Font.FontHeight + i];
			for (j = 0; j < Font.FontWidth; j++)
			{
				if ((b << j) & 0x8000) 
				{
					ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
				} 
				else 
				{
					ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
				}
			}
		}
		
		// 当前绘制光标往左移
		SSD1306.CurrentX += Font.FontWidth;
	}
}

void ssd1306_ClearScreen(void)
{
	memset(SSD1306_Buffer, 0, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
	ssd1306_SetCursor(0,0);
	ssd1306_UpdateScreen();
}












