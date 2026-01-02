/******************************************************************************
 *
 * Project: Smart_Home
 *
 * File Name: Control_APP.c
 *
 * Author: Kerolous_Labib_Georgy
 *
 * Description:
 * ------------
 * This file implements the main control logic for a Smart Home system.
 * It monitors temperature and distance sensors, controls DC motors (windows),
 * detects system faults, and communicates with another module via UART.
 *
 *******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "dc_motor.h"
#include "ultrasonic.h"
#include "lm35_sensor.h"
#include "twi.h"
#include "external_eeprom.h"
#include "adc.h"
#include "gpio.h"

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/

/* UART command bytes */
#define START_MONITORING     1
#define DISPLAY_VALUES       2
#define DETECT_FAULTS        3
#define STOP_MONITORING      4

/* Communication flags */
#define READY 0XFF
#define DONE 0XDF
#define SYNC_BYTE 0xAA
#define END_BYTE   'T'
#define ACK 0x05

/* Critical limits for sensors */
#define CRITICAL_TEMP             90     // Temperature threshold in °C
#define CRITICAL_DISTANCE         10     // Minimum safe distance in cm

/* Diagnostic Trouble Codes (DTC) */
#define DTC_P001 0x01 /* Distance too close */
#define DTC_P002 0x02 /* Overheat */

/* Window button pin mapping */
#define WIN1_OPEN_PORT         PORTD_ID
#define WIN1_OPEN_PIN          PIN2
#define WIN1_CLOSE_PORT        PORTD_ID
#define WIN1_CLOSE_PIN         PIN3

#define WIN2_OPEN_PORT         PORTD_ID
#define WIN2_OPEN_PIN          PIN4
#define WIN2_CLOSE_PORT        PORTD_ID
#define WIN2_CLOSE_PIN         PIN5

/* Button states */
typedef enum {
	BUTTON_RELEASED,
	BUTTON_PRESSED
} BUTTON_STATE;

/* EEPROM max memory address */
#define EEPROM_MAX_ADDRESS 0x07FF

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

volatile uint8 g_Monitoring = 0;          // System monitoring flag
volatile uint8 g_tempValue = 0;           // Current temperature reading
volatile uint16 g_distanceValue = 0;      // Current distance reading
volatile uint8 g_win1_State = 0;          // Window 1 state (open/close)
volatile uint8 g_win2_State = 0;          // Window 2 state (open/close)
volatile uint8 g_distanceLogged = 0;      // Distance fault logged flag
volatile uint8 g_temperatureLogged = 0;   // Temperature fault logged flag
volatile uint8 g_faultCount = 0;          // Number of stored faults

static uint16 EEPROM_addressWrite = 0X0000;   // EEPROM write pointer
uint16 EEPROM_addressRead = 0X0000;           // EEPROM read pointer
uint8 EEPROM_byte;                            // EEPROM buffer

/*******************************************************************************
 *                           Configuration Structs                             *
 *******************************************************************************/

/* UART communication settings */
UART_ConfigType UART_Config = {
	.bit_data = UART_8_BIT_DATA,
	.parity = UART_PARITY_DISABLED,
	.stop_bit = UART_ONE_STOP_BIT,
	.baud_rate = 9600
};

/* I2C (TWI) configuration for EEPROM communication */
TWI_ConfigType TWI_Config = {
	.address = 0X01,
	.bit_rate = TWI_FAST_MODE
};

/* ADC configuration for analog sensors (temperature) */
ADC_typeConfig ADC_config = {
	.reference = ADC_REF_AVCC,
	.prescaler = ADC_PRESCALER_64
};

/* DC Motor configuration (Window 1) */
MOTOR_typeConfig MOTOR1_typeconfig = {
	.PORT_ID = PORTB_ID,
	.IN1_ID = PIN0,
	.IN2_ID = PIN1,
	.EN_ID = PIN3
};

/* DC Motor configuration (Window 2) */
MOTOR_typeConfig MOTOR2_typeconfig = {
	.PORT_ID = PORTB_ID,
	.IN1_ID = PIN4,
	.IN2_ID = PIN5,
	.EN_ID = PIN3
};

/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/
void CONTROL_sendPack(void);
void CONTROL_winState(void);
void detectFaults(void);
void readSensors(void);
void CONTROL_sendFaults(void);

/*******************************************************************************
 *                                main Function                                *
 *******************************************************************************/
int main(void){

//    /* Clear EEPROM fault logs at startup */
//    EEPROM_writeByte(0x0000, 0x00);
//    _delay_ms(10);
//    EEPROM_writeByte(0x000, 0x00);
//    _delay_ms(10);

	/* === Configure button pins as inputs === */
	GPIO_setupPinDirection(WIN1_OPEN_PORT, WIN1_OPEN_PIN, PIN_INPUT);
	GPIO_setupPinDirection(WIN1_CLOSE_PORT, WIN1_CLOSE_PIN, PIN_INPUT);
	GPIO_setupPinDirection(WIN2_OPEN_PORT, WIN2_OPEN_PIN, PIN_INPUT);
	GPIO_setupPinDirection(WIN2_CLOSE_PORT, WIN2_CLOSE_PIN, PIN_INPUT);

	SREG |= (1 << 7); /* Enable global interrupts */

	uint8 keyValue = 0;  // Store received UART command

	/* Initialize peripherals */
	ADC_init(&ADC_config);
	UART_init(&UART_Config);
	TWI_init(&TWI_Config);

	for(;;){
		CONTROL_winState(); // Check and control windows

		/* Process UART commands */
		if(UART_dataAvailable()){
			keyValue = UART_recieveByte();   // Receive command
			UART_sendByte(ACK);              // Acknowledge reception

			switch(keyValue){

			case START_MONITORING:
				Ultrasonic_init();
				DcMotor_Init(&MOTOR1_typeconfig);
				DcMotor_Init(&MOTOR2_typeconfig);
				g_Monitoring = 1;
				break;

			case DISPLAY_VALUES:
				CONTROL_sendPack();           // Send sensor data packet
				break;

			case DETECT_FAULTS:
				CONTROL_sendFaults();          // Send logged faults
				g_distanceLogged = 0;
				g_temperatureLogged = 0;
				break;

			case STOP_MONITORING:
				g_Monitoring = 0;
				break;

			default:
				break;
			}
			keyValue = 0; /* Reset command after processing */
		}

		/* Monitoring mode: read sensors and detect faults */
		if(g_Monitoring){
			readSensors();
			detectFaults();
			_delay_ms(100);
		}
		else{
			_delay_ms(50);
		}
	}
}

/*******************************************************************************
 *                           Function Definitions                              *
 *******************************************************************************/

/*
 * Function: CONTROL_sendPack
 * ---------------------------
 * Sends current sensor and window states over UART to the control panel.
 */
void CONTROL_sendPack(void)
{
	g_tempValue = LM35_getTemperature();

	UART_sendByte((uint8)(g_distanceValue >> 8));  // Send high byte of distance
	while(UART_recieveByte() != ACK);

	UART_sendByte((uint8)(g_distanceValue & 0xFF)); // Send low byte
	while(UART_recieveByte() != ACK);

	UART_sendByte(g_tempValue);                     // Send temperature
	while(UART_recieveByte() != ACK);

	UART_sendByte(g_win1_State);                    // Send window 1 state
	while(UART_recieveByte() != ACK);

	UART_sendByte(g_win2_State);                    // Send window 2 state
	while(UART_recieveByte() != ACK);
}

/*
 * Function: readSensors
 * ----------------------
 * Reads distance and temperature sensors and updates global values.
 */
void readSensors(void)
{
	g_distanceValue = Ultrasonic_readDistance();
	g_tempValue = LM35_getTemperature();

	CONTROL_winState();
}

/*
 * Function: CONTROL_winState
 * ---------------------------
 * Controls both windows (open/close) based on button inputs.
 */
void CONTROL_winState(void)
{
	uint8 win1_open = GPIO_readPin(WIN1_OPEN_PORT, WIN1_OPEN_PIN);
	uint8 win1_close = GPIO_readPin(WIN1_CLOSE_PORT, WIN1_CLOSE_PIN);
	uint8 win2_open = GPIO_readPin(WIN2_OPEN_PORT, WIN2_OPEN_PIN);
	uint8 win2_close = GPIO_readPin(WIN2_CLOSE_PORT, WIN2_CLOSE_PIN);

	/* === WINDOW 1 CONTROL === */
	if(win1_open == BUTTON_PRESSED && win1_close == BUTTON_RELEASED){
		DcMotor_Rotate(&MOTOR1_typeconfig, CW, 100);     // Open window
		_delay_ms(1000);
		DcMotor_Rotate(&MOTOR1_typeconfig, STOP, 100);
		g_win1_State = 1;                                // Mark as open
	}
	else if(win1_open == BUTTON_RELEASED && win1_close == BUTTON_PRESSED){
		DcMotor_Rotate(&MOTOR1_typeconfig, A_CW, 100);   // Close window
		_delay_ms(1000);
		DcMotor_Rotate(&MOTOR1_typeconfig, STOP, 100);
		g_win1_State = 0;                                // Mark as closed
	}

	/* === WINDOW 2 CONTROL === */
	if(win2_open == BUTTON_PRESSED && win2_close == BUTTON_RELEASED){
		DcMotor_Rotate(&MOTOR2_typeconfig, CW, 100);
		_delay_ms(1000);
		DcMotor_Rotate(&MOTOR2_typeconfig, STOP, 100);
		g_win2_State = 1;
	}
	else if(win2_open == BUTTON_RELEASED && win2_close == BUTTON_PRESSED){
		DcMotor_Rotate(&MOTOR2_typeconfig, A_CW, 100);
		_delay_ms(1000);
		DcMotor_Rotate(&MOTOR2_typeconfig, STOP, 100);
		g_win2_State = 0;
	}
}

/*
 * Function: detectFaults
 * -----------------------
 * Detects and logs critical temperature or distance faults to EEPROM.
 */
void detectFaults(void)
{
	g_distanceValue = Ultrasonic_readDistance();
	g_tempValue = LM35_getTemperature();
	/* Distance too close fault */
	if ((g_distanceValue < CRITICAL_DISTANCE) && (!g_distanceLogged)){
		if(EEPROM_addressWrite <= EEPROM_MAX_ADDRESS){
			if(EEPROM_writeByte(EEPROM_addressWrite, DTC_P001) == SUCCESS){
				EEPROM_addressWrite++;
				g_distanceLogged = 1;
			}
			_delay_ms(10);
		}
	}

	/* Overheating fault */
	if ((g_tempValue > CRITICAL_TEMP) && (!g_temperatureLogged)){
		if(EEPROM_addressWrite <= EEPROM_MAX_ADDRESS){
			if(EEPROM_writeByte(EEPROM_addressWrite, DTC_P002) == SUCCESS){
				EEPROM_addressWrite++;
				g_temperatureLogged = 1;
			}
			_delay_ms(10);
		}
	}
}

/*
 * Function: CONTROL_sendFaults
 * -----------------------------
 * Reads fault codes from EEPROM and sends them to the controller via UART.
 */
void CONTROL_sendFaults(void)
{
	uint8 faultCode = 0;
	EEPROM_addressRead = 0x0000;

	while(EEPROM_addressRead <= EEPROM_addressWrite)
	{
		if(EEPROM_readByte(EEPROM_addressRead, &faultCode) == SUCCESS)
		{
			if(faultCode == 0xFF || faultCode == 0x00) {
				break;  // Empty memory location
			}
			UART_sendByte(faultCode);
			while(UART_recieveByte() != ACK);
		}
		else {
			break;  // Communication error
		}
		EEPROM_addressRead++;
		_delay_ms(10);
	}

	/* End of transmission */
	UART_sendByte(END_BYTE);
	while(UART_recieveByte() != ACK);

	/* Reset if EEPROM is full */
	if(EEPROM_addressWrite > EEPROM_MAX_ADDRESS)
	{
		EEPROM_addressWrite = 0x0000;
	}
}
