/*
 * lcd.h
 *
 *  Created on: Sep 13, 2025
 *      Author: Kerolous-Labib
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_

/*******************************************************************************
 *                                Libraries                                    *
 *******************************************************************************/
#include "gpio.h"
#include <util/delay.h>
#include <stdlib.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define LCD_DATA_BITS_MODE                      8
#define LCD_ROWS                                4
#define LCD_COLUMNS                             16


/*******************************************************************************
 *                                Safety Condition                             *
 *******************************************************************************/
#if((LCD_DATA_BITS_MODE != 8) && (LCD_DATA_BITS_MODE != 4))
	#warning "Number Of Data bits should be equal to 4 or 8"
#endif


#define LCD_RS_PORT                             PORTB_ID
#define LCD_RS_PIN	                            PIN2
#define LCD_E_PORT                              PORTB_ID
#define LCD_E_PIN                               PIN7

#define LCD_DATA_PORT                           PORTC_ID
#define LCD_DATA_PIN0                           PIN0
#define LCD_DATA_PIN1                           PIN1
#define LCD_DATA_PIN2                           PIN2
#define LCD_DATA_PIN3                           PIN3

#define LCD_8_BIT_MODE                          0x38
#define LCD_4_BIT_MODE                          0x28
#define LCD_CURSOR_OFF                          0x0C
#define LCD_CURSOR_ON                           0x0E
#define LCD_CLEAR_SCREEN                        0x01
#define LCD_SHIFT_DISPLAY_LEFT                  0x18
#define LCD_SHIFT_DISPLAY_RIGHT                 0x1C
#define LCD_COURSOR_POSITION                    0x80
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/******************************************************************************
 * Description:
 * Initializes the LCD by configuring the control and data pins as outputs according to the bit Mode,
 * then sending the necessary commands to set the LCD mode and clear the screen.
 *
 ******************************************************************************/
void LCD_init(void);

/******************************************************************************
 * Description:
 * Sending Commands to the LCD by configuring the Register Select and Enable,
 * then sending the necessary commands to the LCD according to the Data bits.
  ******************************************************************************/
void LCD_SendCommand(uint8 command);

/******************************************************************************
 * Description:
 * Sending character to the LCD by configuring the Register Select and Enable,
 * then sending the necessary character to the LCD according to the Data bits.
 ******************************************************************************/
void LCD_DisplayCharacter(uint8 character);

/******************************************************************************
 * Description:
 * Sending string to the LCD by reaching the string Character by Character,
 * then sending the character to the LCD according to the Data bits.
 *
 ******************************************************************************/
void LCD_displayString(const char *Str);

/******************************************************************************
 * Description:
 * Moves the cursor to a specific row and column on the LCD.
 * - Calculates the address based on the row and column.
 * - Sends the "Set LCD address" command.
 ******************************************************************************/
void LCD_moveCursor(uint8 row,uint8 col);

/******************************************************************************
 * Description:
 * Displays a string starting at a given row and column.
 * - Calls LCD_moveCursor() to position the cursor.
 * - Calls LCD_displayString() to print the string.
 ******************************************************************************/
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);

/******************************************************************************
 * Description:
 * Converts an integer value into a string and displays it on the LCD.
 * - Uses itoa(data, Str, base) for conversion:
 * 	(uses buffer array to save the value of ASCII of the integers)
 * 	(choose the base you want)
 * - Calls LCD_displayString() to display the result.
 *
 ******************************************************************************/
void LCD_displayInteger(uint8 data);

/******************************************************************************
 * Description:
 * Clears the LCD display and returns the cursor to the home position.
 * - Sends the clear display command (0x01).
 ******************************************************************************/
void LCD_clearScreen(void);

/******************************************************************************
 * Description:
 * Converts a floating-point number into a string with the specified precision
 * (number of digits after the decimal point) and displays it on the LCD.
 ******************************************************************************/
void LCD_displayFloat(float data, uint8 precision);

#endif /* SRC_LCD_H_ */
