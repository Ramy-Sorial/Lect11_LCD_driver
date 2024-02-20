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
#define LCD_DISP_ON 0x01
#define LCD_DISP_OFF 0x00

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

/// @fn void Alcd_Init(Alcd_t*, uint8_t, uint8_t)
/// @brief
/// Initializes the display panel
/// @param lcd ptr to the lcd struct
/// @param Lines number of lines
/// @param Chars number of characters per line
void Alcd_Init(Alcd_t *lcd, uint8_t Lines, uint8_t Chars);

/// @fn void Alcd_Put_n(Alcd_t*, char*, uint8_t)
/// @brief
/// Displays a string on the panel
/// @param lcd ptr to the lcd struct
/// @param text string to be displayed
/// @param len length of the string to be displayed
void Alcd_Put_n(Alcd_t *lcd, char *text, uint8_t len);
void Alcd_PutAt_n(Alcd_t *lcd, uint8_t Row, uint8_t Col, char *text,
		uint8_t len);

/// @fn void Alcd_Clear(Alcd_t*)
/// @brief
///	clears the display panel
/// @param lcd ptr to the lcd struct
void Alcd_Clear(Alcd_t *lcd);

/// @fn void Alcd_Display_Control(Alcd_t*, uint8_t, uint8_t, uint8_t)
/// @brief
///	Used to send commands to the display
/// @param lcd ptr to the lcd struct
/// @param ON_OFF 1 for ON and 0 for OFF
/// @param CUR_ON_OFF 1 to turn on the cursor
/// @param BLINK_ON_OFF 1 to blink the cursor
void Alcd_Display_Control(Alcd_t *lcd, uint8_t ON_OFF, uint8_t CUR_ON_OFF,
		uint8_t BLINK_ON_OFF);

/// @fn void Alcd_CursorAt(Alcd_t*, uint8_t, uint8_t)
/// @brief
///	used to move the cursor to a certain location (where the text should start)
/// @param lcd ptr to the lcd struct
/// @param Row the line number, starting at 0
/// @param Col the column number, starting at 0
void Alcd_CursorAt(Alcd_t *lcd, uint8_t Row, uint8_t Col);

/// @fn void Alcd_CreateChar(Alcd_t*, uint8_t, uint8_t[])
/// @brief
///	Used to create a special character inside the display (ASCII characters 0-7 are empty to be configureed)
/// @param lcd ptr to the lcd struct
/// @param Location location inside the Character Generator RAM  (CGRAM)
/// @param Map a uint8 array of the pixel data, refer to the datasheet for correct assignment
void Alcd_CreateChar(Alcd_t *lcd, uint8_t Location, uint8_t Map[]);

/// @fn void Alcd_PutChar(Alcd_t*, char)
/// @brief
///	puts a single character on the display
/// @param lcd ptr to the lcd struct
/// @param chr character to be displayed
void Alcd_PutChar(Alcd_t *lcd, char chr);

/// @fn int Str_Len(char*)
/// @brief
///	alternate function to strlen to avoid including string.h which is not available for certain platforms (for ex. linux kernel drivers)
/// @param string string to be measured
/// @return the string length (excl. the null characters)
int Str_Len(char *string);

/// @def Alcd_Put
/// @brief
/// puts a string on the display in the current position (useful when appending the data on the display)
#define Alcd_Put(lcd, text) Alcd_PutAt_n(lcd, Row, Col, text, Str_Len(text))

/// @def Alcd_PutAt
/// @brief
/// puts a string on the display without the need to specify the length
#define Alcd_PutAt(lcd, Row, Col, text) Alcd_PutAt_n(lcd, Row, Col, text, Str_Len(text))

#endif /* HD44780_LCD_HD44780_LCD_H_ */
