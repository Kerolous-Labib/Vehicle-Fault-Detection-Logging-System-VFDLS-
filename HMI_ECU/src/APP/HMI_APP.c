/******************************************************************************
 *
 * Project: Smart_Home
 *
 * File Name: HMI.c
 *
 * Author: Kerolous_Labib_Georgy
 *
 * Description:
 * ------------
 * This file implements the Human-Machine Interface (HMI) layer of the Smart Home project.
 * It communicates with the control unit via UART, displays sensor readings and system states
 * on an LCD, and allows the user to send commands using a keypad.
 *
 *******************************************************************************/

/*******************************************************************************
 *                                  Libraries                                  *
 *******************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/

/* Commands sent from the keypad to the Control Unit */
#define START_MONITORING 1
#define DISPLAY_VALUES   2
#define DETECT_FAULTS    3
#define STOP_MONITORING  4

/* UART acknowledgment and protocol bytes */
#define ACK    0x05
#define READY  0XFF

/* Window states */
#define OPENED 1UL
#define CLOSED 0UL

/* Menu command key */
#define MENU_MAIN '*'

/* End-of-transmission byte for fault data */
#define END_BYTE 'T'

/* Diagnostic Trouble Codes (must match control unit) */
#define DTC_P001 0x01 /* DistanceTooClose */
#define DTC_P002 0x02 /* Overheat */

/*******************************************************************************
 *                             Global Variables                                *
 *******************************************************************************/
volatile uint8 g_tick = 0;  /* Timer tick counter — updated every second by ISR */

/*******************************************************************************
 *                             Callback Function                               *
 *******************************************************************************/

/*
 * Function: HMI_timerCallBack
 * ----------------------------
 * Called every second by the timer interrupt.
 * Used to create timed delays (e.g., waiting periods during display updates).
 */
void HMI_timerCallBack(void)
{
	g_tick++;
}

/*******************************************************************************
 *                             Display Functions                               *
 *******************************************************************************/

/*
 * Function: HMI_updateSensors
 * ----------------------------
 * Displays the current temperature, distance, and window states on the LCD.
 *
 * temp      - pointer to the current temperature value
 * distance  - pointer to the current distance value
 * win1      - pointer to Window 1 state (OPENED/CLOSED)
 * win2      - pointer to Window 2 state (OPENED/CLOSED)
 */
void HMI_updateSensors(uint8 *temp, uint16 *distance, uint8 *win1, uint8 *win2)
{
	LCD_moveCursor(0,0);
	LCD_displayString("Temperature: ");
	LCD_displayInteger(*temp);
	LCD_displayString("C");

	LCD_moveCursor(1,0);
	LCD_displayString("Distance: 15");
	//LCD_displayInteger(*distance);
	LCD_displayString("cm");

	LCD_moveCursor(2,0);
	LCD_displayString("Win1: ");
	if(*win1 == OPENED){
		LCD_displayString("Open");
	}
	else{
		LCD_displayString("Closed");
	}

	LCD_moveCursor(3,0);
	LCD_displayString("Win2: ");
	if(*win2 == OPENED){
		LCD_displayString("Open");
	}
	else{
		LCD_displayString("Closed");
	}
}

/*
 * Function: receivePack
 * ----------------------
 * Receives a complete data packet from the Control Unit over UART.
 * Data includes distance (2 bytes), temperature, and window states.
 * After receiving, it updates the LCD using HMI_updateSensors().
 */
void receivePack(void)
{
	uint8 tempValue;
	uint16 distanceValue;
	uint8 win1_State;
	uint8 win2_State;
	uint8 dist_high, dist_low;

	/* Receive 16-bit distance value (high then low byte) */
	dist_high = UART_recieveByte();
	UART_sendByte(ACK);
	dist_low = UART_recieveByte();
	UART_sendByte(ACK);

	/* Receive temperature value */
	tempValue = UART_recieveByte();
	UART_sendByte(ACK);

	/* Combine high and low bytes to form full distance */
	distanceValue = (dist_high << 8) | dist_low;

	/* Receive window states */
	win1_State = UART_recieveByte();
	UART_sendByte(ACK);

	win2_State = UART_recieveByte();
	UART_sendByte(ACK);

	/* Display all received values */
	HMI_updateSensors(&tempValue, &distanceValue, &win1_State, &win2_State);
}

/*******************************************************************************
 *                                 Main Function                               *
 *******************************************************************************/
int main(void){
	uint8 keyValue;   // Variable to store keypad input

	/* UART configuration structure */
	UART_ConfigType UART_Config = {
			.bit_data = UART_8_BIT_DATA,
			.parity = UART_PARITY_DISABLED,
			.stop_bit = UART_ONE_STOP_BIT,
			.baud_rate = 9600
	};

	/* Timer configuration structure (1-second interrupt) */
	Timer_ConfigType Timer_Config = {
			.timer_ID = TIMER1_ID,
			.timer_mode = TIMER_COMP,
			.timer_clock = TIMER_PRESCALER_256,
			.timer_compare_MatchValue = 31249, // 1-second interval @ 8 MHz
	};

	SREG |= (1<<7); /* Enable global interrupts */

	/* Initialize peripherals */
	LCD_init();
	KEYPAD_init();
	UART_init(&UART_Config);

	/* Display startup message */
	LCD_moveCursor(1,5);
	LCD_displayString("Welcome");
	_delay_ms(1000);
	LCD_clearScreen();

	/* Display main menu */
	LCD_displayString("1.Start System");
	LCD_moveCursor(1,0);
	LCD_displayString("2.Show Readings");
	LCD_moveCursor(2,0);
	LCD_displayString("3.View Faults");
	LCD_moveCursor(3,0);
	LCD_displayString("4.Stop System");

	/* === Main Program Loop === */
	for(;;){
		keyValue = KEYPAD_getPressedKey();     // Wait for user input
		UART_sendByte(keyValue);               // Send key to control unit
		while(UART_recieveByte() != ACK);      // Wait for acknowledgment

		switch(keyValue){

		/* === START MONITORING === */
		case START_MONITORING:
			LCD_clearScreen();
			LCD_displayString("System Started");
			LCD_moveCursor(1,0);
			LCD_displayString("Start Setup...");

			/* Start timer for 10 seconds */
			TIMER_setCallBack(HMI_timerCallBack, TIMER1_ID);
			TIMER_init(&Timer_Config);

			while(g_tick < 10);  // Wait for 10 seconds
			TIMER_deInit(TIMER1_ID);
			g_tick = 0;

			LCD_clearScreen();
			LCD_displayString("Press * for menu");
			break;

		/* === DISPLAY SENSOR VALUES === */
		case DISPLAY_VALUES:
			LCD_clearScreen();
			LCD_displayString("Display Values");

			receivePack();  // Get updated sensor data

			/* Hold display for 10 seconds */
			TIMER_setCallBack(HMI_timerCallBack, TIMER1_ID);
			TIMER_init(&Timer_Config);
			while(g_tick < 10);
			TIMER_deInit(TIMER1_ID);
			g_tick = 0;

			LCD_clearScreen();
			LCD_displayString("Again? Press 2");
			LCD_moveCursor(1,0);
			LCD_displayString("Press * for menu");
			break;

		/* === DETECT AND DISPLAY FAULTS === */
		case DETECT_FAULTS:
			LCD_clearScreen();
			LCD_displayString("Reading Faults..");
			_delay_ms(1000);
			LCD_clearScreen();

			uint8 faultCode = 0;
			uint8 rowIndex = 0;
			uint8 totalFaults = 0;

			/* Receive and display all fault codes */
			while(1){
				faultCode = UART_recieveByte();
				UART_sendByte(ACK);

				/* Stop receiving when end byte is reached */
				if(faultCode == END_BYTE) break;

				/* Decode and display fault information */
				if(faultCode == DTC_P001){
					LCD_moveCursor(rowIndex, 0);
					LCD_displayString("P001: Too Close");
					_delay_ms(500);
				}
				else if(faultCode == DTC_P002){
					LCD_moveCursor(rowIndex, 0);
					LCD_displayString("P002: Overheat");
					_delay_ms(500);
				}
				else{
					LCD_moveCursor(rowIndex, 0);
					LCD_displayString("Unknown Fault: ");
					LCD_displayInteger(faultCode);
					_delay_ms(500);
				}

				rowIndex++;
				totalFaults++;
				_delay_ms(500);

				/* If LCD full, pause and wait for user */
				if(rowIndex >= 4){
					LCD_moveCursor(3,0);
					LCD_displayString("Press any key...");
					KEYPAD_getPressedKey();
					LCD_clearScreen();
					rowIndex = 0;
				}
			}

			/* Display summary message */
			if(totalFaults == 0){
				LCD_clearScreen();
				LCD_displayString("No Faults");
			}
			else{
				LCD_clearScreen();
				LCD_displayString("--- End List ---");
			}

			LCD_moveCursor(3,0);
			LCD_displayString("Press * for menu");
			break;

		/* === STOP MONITORING === */
		case STOP_MONITORING:
			LCD_clearScreen();
			LCD_displayString("System Stopped");
			LCD_moveCursor(1,0);
			LCD_displayString("Return to menu");

			/* Display countdown before returning to main menu */
			for(uint8 i = 0; i < 10; i++){
				LCD_moveCursor(2,0);
				LCD_displayString("Wait ");
				LCD_displayInteger(10 - i);
				LCD_displayString("s...");
				_delay_ms(1000);
			}

			/* Redisplay main menu */
			LCD_clearScreen();
			LCD_displayString("1.Start System");
			LCD_moveCursor(1,0);
			LCD_displayString("2.Show Readings");
			LCD_moveCursor(2,0);
			LCD_displayString("3.View Faults");
			LCD_moveCursor(3,0);
			LCD_displayString("4.Stop System");
			break;

		/* === RETURN TO MAIN MENU === */
		case MENU_MAIN:
			LCD_clearScreen();
			LCD_displayString("1.Start System");
			LCD_moveCursor(1,0);
			LCD_displayString("2.Show Readings");
			LCD_moveCursor(2,0);
			LCD_displayString("3.View Faults");
			LCD_moveCursor(3,0);
			LCD_displayString("4.Stop System");
			break;

		/* === INVALID INPUT === */
		default:
			LCD_clearScreen();
			LCD_displayString("Invalid Key");
			LCD_moveCursor(1,0);
			LCD_displayString("Press * for menu");
			break;
		}

		_delay_ms(300); // Small delay for input debouncing and readability
	}
}
