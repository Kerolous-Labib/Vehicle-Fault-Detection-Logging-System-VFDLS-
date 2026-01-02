/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver (Polling-based)
 *
 * Author: Kerolous Labib
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h"       /* UART Registers */
#include "common_macros.h" /* Bit manipulation macros */


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize UART:
 * - Configure frame format (data bits, parity, stop bits)
 * - Enable TX & RX
 * - Configure baud rate (double speed mode)
 */
void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint16 ubrr_value = 0;

	/* Enable double transmission speed */
	UCSRA = (1 << U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable RX Complete Interrupt
	 * TXCIE = 0 Disable TX Complete Interrupt
	 * UDRIE = 0 Disable UDR Empty Interrupt
	 * RXEN  = 1 Receiver Enable
	 * TXEN  = 1 Transmitter Enable
	 * UCSZ2 = Configured for 9-bit mode only
	 ***********************************************************************/
	UCSRB = (1 << RXEN) | (1 << TXEN);
	if(Config_Ptr->bit_data == UART_9_BIT_DATA)
		SET_BIT(UCSRB, UCSZ2);
	else
		CLEAR_BIT(UCSRB, UCSZ2);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 (Write to UCSRC)
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = Parity mode
	 * USBS    = Stop bits
	 * UCSZ1:0 = Data bits
	 * UCPOL   = 0 (Used only for synchronous mode)
	 ***********************************************************************/
	UCSRC = (1 << URSEL)
			| ((Config_Ptr->parity) << UPM0)
			| ((Config_Ptr->stop_bit) << USBS)
			| (GET_BIT(Config_Ptr->bit_data, 0) << UCSZ0)
			| (GET_BIT(Config_Ptr->bit_data, 1) << UCSZ1);

	/* Calculate UBRR value for baud rate (with double speed) */
	ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);

	/* Set baud rate registers */
	UBRRH = ubrr_value >> 8;
	UBRRL = ubrr_value;
}

/*
 * Description :
 * Send one byte through UART (polling-based).
 */
void UART_sendByte(const uint8 data)
{
	/* Wait until UDR register is empty (UDRE = 1) */
	while(BIT_IS_CLEAR(UCSRA, UDRE)) {}

	/* Put data in the transmit buffer */
	UDR = data;
}

/*
 * Description :
 * Receive one byte through UART (polling-based).
 */
uint8 UART_recieveByte(void)
{
	/* Wait until data is received (RXC = 1) */
	while(BIT_IS_CLEAR(UCSRA, RXC)) {}

	/* Return received data from buffer */
	return UDR;
}

/*
 * Description :
 * Send a null-terminated string through UART.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

/*
 * Description :
 * Receive a string until '#' symbol, then replace it with '\0'.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive first byte */
	Str[i] = UART_recieveByte();

	/* Keep receiving until '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* Replace '#' with string terminator */
	Str[i] = '\0';
}

/*
 * Description :
 * Check if new data is available (non-blocking polling).
 * Returns 1 if RXC = 1 (data received), else 0.
 */
uint8 UART_dataAvailable(void)
{
	return BIT_IS_SET(UCSRA, RXC);
}
