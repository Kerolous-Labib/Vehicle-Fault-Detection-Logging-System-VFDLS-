/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver (Polling-based)
 *
 * Author: Kerolous Labib
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                                Data Types                                    *
 *******************************************************************************/

typedef enum
{
	UART_5_BIT_DATA,
	UART_6_BIT_DATA,
	UART_7_BIT_DATA,
	UART_8_BIT_DATA,
	UART_9_BIT_DATA = 7,
}UART_BitDataType;

typedef enum
{
	UART_PARITY_DISABLED,
	UART_RESERVED,
	UART_PARITY_EVEN,
	UART_PARITY_ODD
}UART_ParityType;

typedef enum
{
	UART_ONE_STOP_BIT,
	UART_TWO_STOP_BITS,
}UART_StopBitType;

typedef uint32 UART_BaudRateType;

typedef struct {
	UART_BitDataType bit_data;
	UART_ParityType parity;
	UART_StopBitType stop_bit;
	UART_BaudRateType baud_rate;
}UART_ConfigType;

/* Global configuration structure instance */
extern UART_ConfigType UART_Config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the UART device by:
 * 1. Setting frame format (data bits, parity, stop bits)
 * 2. Enabling transmitter and receiver
 * 3. Setting baud rate
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Send one byte to another UART device (polling-based).
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Receive one byte from another UART device (polling-based).
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send a null-terminated string through UART.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive a string through UART until the '#' symbol.
 * The '#' is replaced by '\0' in the received buffer.
 */
void UART_receiveString(uint8 *Str);

/*
 * Description :
 * Check if new data has been received (non-blocking check).
 * Returns 1 if RXC flag is set, 0 otherwise.
 */
uint8 UART_dataAvailable(void);

#endif /* UART_H_ */
