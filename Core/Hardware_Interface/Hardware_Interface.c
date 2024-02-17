/*
 * LCD_HW_Interface.c
 *
 *  Created on: Feb 5, 2024
 *      Author: ramys
 */

#include "Hardware_Interface.h"

///
/// The LCD driver structure instance ocntaining the screen configuration
Alcd_t lcd =
{
		.HW_Interface =
			{
					.usDelay = usDelay,
					.Write_HalfByte = WriteHalfByte,
					.RS_SET = RS_Set,
					.EN_SET = EN_SET
			},
};

//define the Hardware interface function to be used indie the driver
void usDelay(uint16_t delay)
{
	DWT_Delay_us(delay);
}
void WriteHalfByte(uint8_t data)
{
	GPIOA->ODR &= ~(0xf << LCD_D4_Pin);
	GPIOA->ODR |= data << LCD_D4_Pin;
}
void RS_Set(uint8_t ON_OFF)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, ON_OFF);
}
void EN_SET(uint8_t ON_OFF)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, ON_OFF);
}
void InitLCD()
{
	//initialize the ARM CM3 delay function
	DWT_Delay_Init();

	//enable the GPIOA peripheral clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//configure GPIO pins connected to the LCS as outputs
	GPIO_InitTypeDef G =
	{
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,
			.Speed = GPIO_SPEED_FREQ_LOW
	};

	HAL_GPIO_Init(GPIOA, &G);

	//Initialize the LCD driver itself after initializing
	//the MCAL peripheral and linking the proper function pointers
	Alcd_Init(&lcd, 2, 16);
}
