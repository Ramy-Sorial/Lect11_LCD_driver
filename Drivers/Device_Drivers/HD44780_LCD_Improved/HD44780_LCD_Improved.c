/*
 * HD44780_LCD.c
 *
 *  Created on: Feb 3, 2024
 *      Author: ramys
 */

#include "HD44780_LCD_Improved.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define SendByte(cmd_data, value) Alcd_SendByte(lcd, cmd_data, value)
#define PulseEn                     \
  do                                \
  {                                 \
    lcd->HW_Interface.EN_SET(0);    \
    lcd->HW_Interface.usDelay(1);   \
    lcd->HW_Interface.EN_SET(1);    \
    lcd->HW_Interface.usDelay(1);   \
    lcd->HW_Interface.EN_SET(0);    \
    lcd->HW_Interface.usDelay(100); \
  } while (0)

void Alcd_Display(Alcd_t *lcd, uint8_t ON_OFF);

static inline void Alcd_SendByte(Alcd_t *lcd, uint8_t CMD_Data, uint8_t value)
{
	lcd->HW_Interface.RS_SET(CMD_Data);
	// send the higher 4 bits
	lcd->HW_Interface.Write_HalfByte(value >> 4);
	// pulse the enable pin
	PulseEn
	;
	lcd->HW_Interface.Write_HalfByte(value);
	PulseEn
	;
}

void Alcd_Init(Alcd_t *lcd, uint8_t Lines, uint8_t Chars)
{
	uint8_t x;

	lcd->RowOffsets[0] = 0;
	lcd->RowOffsets[1] = 0x40;
	lcd->RowOffsets[2] = 0 + Chars;
	lcd->RowOffsets[3] = 0x40 + Chars;

	lcd->HW_Interface.RS_SET(0);
	lcd->HW_Interface.EN_SET(0);
	lcd->HW_Interface.usDelay(50000);

	// init display in 4-bit mode
	for (x = 0; x < 2; x++)
	{
		lcd->HW_Interface.Write_HalfByte(0x03);
		PulseEn
		;
		lcd->HW_Interface.usDelay(4500);
	}
	lcd->HW_Interface.Write_HalfByte(0x03);
	PulseEn
	;
	lcd->HW_Interface.usDelay(150);
	lcd->HW_Interface.Write_HalfByte(0x02);
	PulseEn
	;

	// finally, set # lines, font size, etc.
	SendByte(0, LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS);

	// turn the display on with no cursor or blinking default
	// lcd->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

	// turn on the display
	Alcd_Display_Control(lcd, LCD_DISP_ON, LCD_CURSOROFF, LCD_BLINKOFF);
	Alcd_Clear(lcd);
}

void Alcd_CursorAt(Alcd_t *lcd, uint8_t Row, uint8_t Col)
{
	SendByte(0, LCD_SETDDRAMADDR | (Col + lcd->RowOffsets[Row]));
}

void Alcd_Put_n(Alcd_t *lcd, char *text, uint8_t len)
{
	for (uint8_t x = 0; x < len; x++)
	{
		SendByte(1, *(text++));
	}
}

void Alcd_PutAt_n(Alcd_t *lcd, uint8_t Row, uint8_t Col, char *text, uint8_t len)
{
	Alcd_CursorAt(lcd, Row, Col);
	Alcd_Put_n(lcd, text, len);
}

void Alcd_Home(Alcd_t *lcd)
{
	SendByte(0, LCD_RETURNHOME);
	lcd->HW_Interface.usDelay(2000);
}

void Alcd_Clear(Alcd_t *lcd)
{
	SendByte(0, LCD_CLEARDISPLAY);
	lcd->HW_Interface.usDelay(2000);
}

void Alcd_Display_Control(Alcd_t *lcd, uint8_t ON_OFF, uint8_t CUR_ON_OFF, uint8_t BLINK_ON_OFF)
{
	lcd->_displaycontrol = 0;
	if (ON_OFF)
	{
		lcd->_displaycontrol |= LCD_DISPLAYON;
	}
	if (CUR_ON_OFF)
	{
		lcd->_displaycontrol |= LCD_CURSORON;
	}
	if (BLINK_ON_OFF)
	{
		lcd->_displaycontrol |= LCD_BLINKON;
	}
	lcd->_displaycontrol |= LCD_DISPLAYON;
	SendByte(0, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void Alcd_CreateChar(Alcd_t *lcd, uint8_t Location, uint8_t Map[])
{
	uint8_t x = 0;
	// only 8 locations are
	Location &= 7;
	SendByte(0, LCD_SETCGRAMADDR | (Location << 3));
	for (x = 0; x < 8; x++)
	{
		SendByte(1, Map[x]);
	}
}

void Alcd_PutChar(Alcd_t *lcd, char chr)
{
	SendByte(1, chr);
}

int Str_Len(char *string)
{
	int len = 0;
	while (*(string++))
	{
		len++;
	}
	return len;
}
