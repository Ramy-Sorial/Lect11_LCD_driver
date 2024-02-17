/*
 * HD44780_LCD.h
 *
 *  Created on: Feb 3, 2024
 *      Author: ramys
 */

#ifndef HD44780_LCD_HD44780_LCD_H_
#define HD44780_LCD_HD44780_LCD_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

typedef struct ALCD {
	uint8_t Lines;
	uint8_t Chars;
	struct {
		void (*usDelay)(uint16_t delay_us);
		/// the top 4 bits are ignored
		/// transfers the status of the 4-bit data to the hardware GPIO pins
		void (*Write_HalfByte)(uint8_t HalfByte);
		/// @brief 1 for set and 0 for reset
		void (*RS_SET)(uint8_t R_S_Stat);
		/// @brief 1 for set and 0 for reset
		void (*EN_SET)(uint8_t EN_Stat);
	} HW_Interface;

	// hold the row offsets based on the line length
	uint8_t RowOffsets[4];
	uint8_t _displaycontrol;
} Alcd_t;

void Alcd_Init(Alcd_t *lcd, uint8_t Lines, uint8_t Chars);

void Alcd_Put_n(Alcd_t *lcd, char *text, uint8_t len);
void Alcd_PutAt_n(Alcd_t *lcd, uint8_t Row, uint8_t Col, char *text,
		uint8_t len);
void Alcd_Clear(Alcd_t *lcd);
void Alcd_Display_Control(Alcd_t *lcd, uint8_t ON_OFF, uint8_t CUR_ON_OFF,
		uint8_t BLINK_ON_OFF);
void Alcd_CursorAt(Alcd_t *lcd, uint8_t Row, uint8_t Col);

void Alcd_CreateChar(Alcd_t *lcd, uint8_t Location, uint8_t Map[]);
void Alcd_PutChar(Alcd_t *lcd, char chr);

int Str_Len(char *string);

#define Alcd_Put(lcd, text) Alcd_PutAt_n(lcd, Row, Col, text, Str_Len(text))

#define Alcd_PutAt(lcd, Row, Col, text) Alcd_PutAt_n(lcd, Row, Col, text, Str_Len(text))

#endif /* HD44780_LCD_HD44780_LCD_H_ */
