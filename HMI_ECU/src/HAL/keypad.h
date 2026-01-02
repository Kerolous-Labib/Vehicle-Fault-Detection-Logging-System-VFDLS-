/******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 * Description: Header file for the Keypad driver
 *
 * Author: Kerolous Labib
 *
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Keypad configurations */
#define KEYPAD_NUM_COLS                   4
#define KEYPAD_NUM_ROWS                   4

/* Keypad Port Configurations */
#define KEYPAD_ROW_PORT_ID                PORTA_ID
#define KEYPAD_FIRST_ROW_PIN_ID           PIN0

#define KEYPAD_COL_PORT_ID                PORTB_ID
#define KEYPAD_FIRST_COL_PIN_ID           PIN4

/* Keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED             LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED            LOGIC_HIGH

/* Debounce delay (ms) */
#define KEYPAD_DEBOUNCE_DELAY_MS          30

/*  */
#define NO_PRESSED_KEY                    0xFF

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize keypad row and column pins
 */
void KEYPAD_init(void);

/*
 * Description :
 * Get the Keypad pressed button (Blocking function — waits until key press)
 */
uint8 KEYPAD_getPressedKey(void);

/*
 * Description :
 * Read the Keypad pressed button (Non-blocking — returns 0xFF if no key is pressed)
 */
uint8 KEYPAD_readKey(void);

#endif /* KEYPAD_H_ */
