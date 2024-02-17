/*
 * LCD_HW_Interface.h
 *
 *  Created on: Feb 5, 2024
 *      Author: ramys
 */

#ifndef DEVICE_DRIVERS_HD44780_LCD_LCD_HW_INTERFACE_H_
#define DEVICE_DRIVERS_HD44780_LCD_LCD_HW_INTERFACE_H_

//include the header file for the LCD driver
#include "../../Drivers/Device_Drivers/HD44780_LCD_Improved/HD44780_LCD_Improved.h"

//include the header file for the ARM CM3 Delay generation function
#include "../../Drivers/ARM_Drivers/Delay_Micros/dwt_stm32_delay.h"

#define LCD_D4_Pin 0
#define LCD_RS_Pin 4
#define LCD_EN_Pin 5

//declare the hardware interface functions to be used by the LCD driver
/// @fn void usDelay(uint16_t)
/// @brief
/// Used to generate a delay in microseconds
/// @param delay the amount of delay in microseconds
void usDelay(uint16_t delay);

/// @fn void WriteHalfByte(uint8_t)
/// @brief
/// Used to write the D4-D7 GPIO logic levels as a 4-bit value
/// @param data the 4-bit data to be written, the upper 4 bits will be ignored
void WriteHalfByte(uint8_t data);

/// @fn void RS_Set(uint8_t)
/// @brief
///	sets the logic level of the RS pin
/// @param ON_OFF 1 for on and 0 for off
void RS_Set(uint8_t ON_OFF);

/// @fn void EN_SET(uint8_t)
/// @brief
/// sets the logic level of the EN pin
/// @param ON_OFF 1 for high and 0 for low
void EN_SET(uint8_t ON_OFF);

/// @fn void InitLCD()
/// @brief
/// Initializes the GPIO pins connected to the LCD panel
void InitLCD();

//defined here as extern to allow all files including this file to see the lcd struct
extern Alcd_t lcd;

#endif /* DEVICE_DRIVERS_HD44780_LCD_LCD_HW_INTERFACE_H_ */
