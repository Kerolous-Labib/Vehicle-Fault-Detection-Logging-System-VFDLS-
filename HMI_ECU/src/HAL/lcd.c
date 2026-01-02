/*
 * lcd.c
 *
 *  Created on: Sep 13, 2025
 *  Author: Kerolous Labib Georgy
 */
#include "lcd.h"

/******************************************************************************
 * Description:
 * Initializes the LCD by configuring the control and data pins as outputs according to the bit Mode,
 * then sending the necessary commands to set the LCD mode and clear the screen.
 *
 ******************************************************************************/

void LCD_init(void)
{
	/* Configure the Register Select as OUTPUT PIN */
	GPIO_setupPinDirection(LCD_RS_PORT, LCD_RS_PIN, PIN_OUTPUT);
	/* Configure the Enable as OUTPUT PIN */
	GPIO_setupPinDirection(LCD_E_PORT, LCD_E_PIN, PIN_OUTPUT);

	/* Select The LCD Mode by changing the LCD_DATA_BITS_MODE from the header file */
	#if(LCD_DATA_BITS_MODE == 8)
		/* Configure the 8_BIT_DATA_PINS as OUTPUT PORT */
		GPIO_setupPortDirection(LCD_DATA_PORT, PORT_OUTPUT);
		_delay_ms(20);
		/* Function Set: 8-bit, 2 Line, 5X7 Dots */
		LCD_SendCommand(LCD_8_BIT_MODE);

	#elif(LCD_DATA_BITS_MODE == 4)
		/* Configure the 4_BIT_DATA_PINS as OUTPUT PINS */
		GPIO_setupPinDirection(LCD_DATA_PORT, LCD_DATA_PIN0, PIN_OUTPUT);
		GPIO_setupPinDirection(LCD_DATA_PORT, LCD_DATA_PIN1, PIN_OUTPUT);
		GPIO_setupPinDirection(LCD_DATA_PORT, LCD_DATA_PIN2, PIN_OUTPUT);
		GPIO_setupPinDirection(LCD_DATA_PORT, LCD_DATA_PIN3, PIN_OUTPUT);
		_delay_ms(20);
		/* Function Set: 4-bit, 2 Line, 5X7 Dots */
		LCD_SendCommand(LCD_4_BIT_MODE);
	#endif

		/* Display ON Cursor OFF */
		LCD_SendCommand(LCD_CURSOR_OFF);
		/* Clear Display */
		LCD_SendCommand(LCD_CLEAR_SCREEN);

}

/******************************************************************************
 * Description:
 * Sending Commands to the LCD by configuring the Register Select and Enable,
 * then sending the necessary commands to the LCD according to the Data bits.
 *
 *Notes:
 *	1. if the LCD is 8_BIT_MODE, then send the command on the port
 *	2. if the LCD is 4_BIT_MODE, then send the higher 4-bits first then the lower 4-bits
 ******************************************************************************/
void LCD_SendCommand(uint8 command)
{
	/* Write A Logic Low on the RS Pin */
	GPIO_writePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_LOW);
	_delay_ms(1);
	/* Write A logic High on the Enable Pin */
	GPIO_writePin(LCD_E_PORT, LCD_E_PIN, LOGIC_HIGH);
	_delay_ms(1);

	#if(LCD_DATA_BITS_MODE == 8)
		/* Send the command on the whole PORT */
		GPIO_writePort(LCD_DATA_PORT, command);

	#elif(LCD_DATA_BITS_MODE == 4)
		/* Get the MSB and Write the Value On the Data Pins */
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN0, GET_BIT(command,4));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN1, GET_BIT(command,5));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN2, GET_BIT(command,6));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN3, GET_BIT(command,7));

		_delay_ms(1);
		/* Disable LCD E=0 */
		GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);

		_delay_ms(1);
		/* Enable LCD E=1 */
		GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_HIGH);

		_delay_ms(1);

		/* Get the LSB and Write the Value On the Data Pins */
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN0, GET_BIT(command,0));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN1, GET_BIT(command,1));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN2, GET_BIT(command,2));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN3, GET_BIT(command,3));

	#endif

	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);								// Write A logic Low on the Enable Pin
	_delay_ms(1);
}

/******************************************************************************
 * Description:
 * Sending character to the LCD by configuring the Register Select and Enable,
 * then sending the necessary character to the LCD according to the Data bits.
 *
 *Notes:
 *	1. if the LCD is 8_BIT_MODE, then send the character on the port
 *	2. if the LCD is 4_BIT_MODE, then send the higher 4-bits first then the lower 4-bits
 ******************************************************************************/
void LCD_DisplayCharacter(uint8 character)
{
	/* Write A Logic High on the RS Pin */
	GPIO_writePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_HIGH);
	_delay_ms(1);
	/* Write A logic High on the Enable Pin */
	GPIO_writePin(LCD_E_PORT, LCD_E_PIN, LOGIC_HIGH);
	_delay_ms(1);

	/* Configuring the Data bits mode */
	#if(LCD_DATA_BITS_MODE == 8)
		/* Send the command on the whole PORT */
		GPIO_writePort(LCD_DATA_PORT, character);

	#elif(LCD_DATA_BITS_MODE == 4)
		/* Get the MSB and Write the Value On the Data Pins */
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN0, GET_BIT(character,4));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN1, GET_BIT(character,5));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN2, GET_BIT(character,6));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN3, GET_BIT(character,7));

		_delay_ms(1);
		/* Disable LCD E=0 */
		GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);

		_delay_ms(1);
		/* Enable LCD E=1 */
		GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_HIGH);

		_delay_ms(1);

		/* Get the LSB and Write the Value On the Data Pins */
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN0, GET_BIT(character,0));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN1, GET_BIT(character,1));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN2, GET_BIT(character,2));
		GPIO_writePin(LCD_DATA_PORT, LCD_DATA_PIN3, GET_BIT(character,3));

	#endif
	_delay_ms(1);
	/* Write a logic Low on the Enable Pin */
	GPIO_writePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);
	_delay_ms(1);
}


/******************************************************************************
 * Description:
 * Sending string to the LCD by reaching the string Character by Character,
 * then sending the character to the LCD according to the Data bits.
 *
 ******************************************************************************/
void LCD_displayString(const char *Str)
{
	/* Loop on the string until reach the NULL and display the character */
	for(uint8 i = 0; *(Str+i) != '\0';i++){
		LCD_DisplayCharacter(*(Str+i));
	}
}

/******************************************************************************
 * Description:
 * Moves the cursor to a specific row and column on the LCD.
 * - Calculates the address based on the row and column.
 * - Sends the "Set LCD address" command.
 *
 * Notes:
 *   row: Row index (0-based).
 *   col: Column index (0-based).
 ******************************************************************************/
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 LCD_Address;
	if((row >= LCD_ROWS) || (col >= LCD_COLUMNS)){
		/* Do Nothing */
	}
	else{
		switch(row){
		case 0:
			LCD_Address = col;
			break;
		case 1:
			LCD_Address = 0x40 + col;
			break;
	#if(LCD_ROWS == 4)
		case 2:
			LCD_Address = 0x10 + col;
			break;
		case 3:
			LCD_Address = 0x50 + col;
			break;
	#endif
		}
		LCD_SendCommand(LCD_COURSOR_POSITION + LCD_Address);
	}
}


/******************************************************************************
 * Description:
 * Displays a string starting at a given row and column.
 * - Calls LCD_moveCursor() to position the cursor.
 * - Calls LCD_displayString() to print the string.
 *
 * Notes:
 *   row: Row index (0-based).
 *   col: Column index (0-based).
 *   Str: Pointer to the null-terminated string.
 ******************************************************************************/
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_moveCursor(row, col);
	LCD_displayString(Str);
}


/******************************************************************************
 * Description:
 * Converts an integer value into a string and displays it on the LCD.
 * - Uses itoa(data, Str, base) for conversion:
 * 	(uses buffer array to save the value of ASCII of the integers)
 * 	(choose the base you want)
 * - Calls LCD_displayString() to display the result.
 *
 ******************************************************************************/
void LCD_displayInteger(uint8 data)
{
	/* Array to save the data ASCII */
	char buffer[20];
	itoa(data, buffer, 10);
	/* Calling the LCD_displayString() to print the String*/
	LCD_displayString(buffer);
}

/******************************************************************************
 * Description:
 * Clears the LCD display and returns the cursor to the home position.
 * - Sends the clear display command (0x01).
 ******************************************************************************/
void LCD_clearScreen(void)
{
	LCD_SendCommand(LCD_CLEAR_SCREEN);
}


/******************************************************************************
 * Description:
 * Converts a floating-point number into a string with the specified precision
 * (number of digits after the decimal point) and displays it on the LCD.
 *
 * Notes:
 *   - Uses dtostrf() to handle float-to-string conversion.
 *   - Useful for displaying sensor values (temperature, voltage, etc.).
 *
 ******************************************************************************/
void LCD_displayFloat(float data, uint8 precision)
{
    char buffer[20];
    dtostrf(data, 6, precision, buffer);
    /* Calling the LCD_displayString() to print the String*/
    LCD_displayString(buffer);
}
