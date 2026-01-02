/******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: Kerolous Labib
 *
 *******************************************************************************/

#include "keypad.h"
#include "gpio.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Private Function Prototypes                            *
 *******************************************************************************/
#if (KEYPAD_NUM_COLS == 3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);
#elif (KEYPAD_NUM_COLS == 4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif

/*******************************************************************************
 *                      Function Definitions                                   *
 *******************************************************************************/

void KEYPAD_init(void)
{
	uint8 i;

	/* Configure all rows as inputs initially */
	for(i = 0; i < KEYPAD_NUM_ROWS; i++)
	{
		GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + i, PIN_INPUT);
	}

	/* Configure all columns as inputs with pull-ups enabled */
	for(i = 0; i < KEYPAD_NUM_COLS; i++)
	{
		GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + i, PIN_INPUT);
		GPIO_writePin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + i, LOGIC_HIGH);
	}
}

/*
 * Description:
 * Scan the keypad once and return the pressed key (or 0xFF if none)
 */
uint8 KEYPAD_readKey(void)
{
	uint8 row, col;

	for(row = 0; row < KEYPAD_NUM_ROWS; row++)
	{
		/* Set current row as output and drive it to KEYPAD_BUTTON_PRESSED (usually LOW) */
		GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + row, PIN_OUTPUT);
		GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + row, KEYPAD_BUTTON_PRESSED);

		for(col = 0; col < KEYPAD_NUM_COLS; col++)
		{
			if(GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + col) == KEYPAD_BUTTON_PRESSED)
			{
				/* Simple debounce */
				_delay_ms(KEYPAD_DEBOUNCE_DELAY_MS);

				if(GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + col) == KEYPAD_BUTTON_PRESSED)
				{
					/* Return mapped key */
					#if (KEYPAD_NUM_COLS == 3)
						return KEYPAD_4x3_adjustKeyNumber((row * KEYPAD_NUM_COLS) + col + 1);
					#elif (KEYPAD_NUM_COLS == 4)
						return KEYPAD_4x4_adjustKeyNumber((row * KEYPAD_NUM_COLS) + col + 1);
					#endif
				}
			}
		}

		/* Reset current row to input before next iteration */
		GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + row, PIN_INPUT);
	}

	return NO_PRESSED_KEY; /* No key pressed */
}

/*
 * Description:
 *  waits until a key is pressed
 */
uint8 KEYPAD_getPressedKey(void)
{
	uint8 key;
	do {
		key = KEYPAD_readKey();
	} while(key == NO_PRESSED_KEY);

	return key;
}

#if (KEYPAD_NUM_COLS == 3)
/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x3 shape
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button = 0;
	switch(button_number)
	{
		case 10: keypad_button = '*'; // ASCII Code of *
				 break;
		case 11: keypad_button = 0;
				 break;
		case 12: keypad_button = '#'; // ASCII Code of #
				 break;
		default: keypad_button = button_number;
				break;
	}
	return keypad_button;
}

#elif (KEYPAD_NUM_COLS == 4)

/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x4 shape
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button = 0;
	switch(button_number)
	{
		case 1: keypad_button = 7;
				break;
		case 2: keypad_button = 8;
				break;
		case 3: keypad_button = 9;
				break;
		case 4: keypad_button = '%'; // ASCII Code of %
				break;
		case 5: keypad_button = 4;
				break;
		case 6: keypad_button = 5;
				break;
		case 7: keypad_button = 6;
				break;
		case 8: keypad_button = '*'; /* ASCII Code of '*' */
				break;
		case 9: keypad_button = 1;
				break;
		case 10: keypad_button = 2;
				break;
		case 11: keypad_button = 3;
				break;
		case 12: keypad_button = '-'; /* ASCII Code of '-' */
				break;
		case 13: keypad_button = 13;  /* ASCII of Enter */
				break;
		case 14: keypad_button = 0;
				break;
		case 15: keypad_button = '='; /* ASCII Code of '=' */
				break;
		case 16: keypad_button = '+'; /* ASCII Code of '+' */
				break;
		default: keypad_button = button_number;
				break;
	}
	return keypad_button;
}

#endif
