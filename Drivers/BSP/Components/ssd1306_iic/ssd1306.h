#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "stm32l4xx_hal.h"
#include "fonts.h"

// i2c port naam in main programma gegenereerd door cube
#define SSD1306_I2C_PORT		hi2c3
// I2C address 
#define SSD1306_I2C_ADDR        0x78
// SSD1306 width in pixels
#define SSD1306_WIDTH           128
// SSD1306 LCD height in pixels
#define SSD1306_HEIGHT          64

//
//	Enum voor de kleuren van het scherm Black en White
//
typedef enum {
	Black = 0x00, /*!< Black color, no pixel */
	White = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR;

//
//	Struct om wijzigingen bij te houden
//
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

#define SDA_IN()  {GPIOC ->MODER&=~(3<<(1*2));GPIOH ->MODER|=0<<1*2;}
#define SDA_OUT() {GPIOC->MODER&=~(3<<(1*2));GPIOH ->MODER|=1<<1*2;}  

#define IIC_SCL_HIGH  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
#define IIC_SCL_LOW   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
#define IIC_SDA_HIGH  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)
#define IIC_SDA_LOW   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)
#define IIC_SDA_WRITE(data)  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, data);

#define READ_SDA      HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)    
//
//	De functies definities van de functies die gebruikt kunnen worden
//
void ssd1306_Init(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_ClearScreen(void);

static void ssd1306_WriteCommand(uint8_t command);
void ssd1306_WriteChineseString(uint8_t index, uint8_t count, FontDef Font, SSD1306_COLOR color);

#endif



