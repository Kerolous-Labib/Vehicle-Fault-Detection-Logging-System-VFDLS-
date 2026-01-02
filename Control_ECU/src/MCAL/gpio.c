/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: Source file for the AVR GPIO driver
 *
 * Author: Kerolous_Labib_Georgy
 *
 *******************************************************************************/

#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use the IO Ports Registers */

/* configure static global Arrays of pointers to Registers  */

/* Direction Register Array (Read/Write) */
static volatile uint8* const DDR_REG[NUM_OF_PORTS] = {&DDRA, &DDRB, &DDRC, &DDRD};
/* Output Register Array (Read/Write) */
static volatile uint8* const PORT_REG[NUM_OF_PORTS] = {&PORTA, &PORTB, &PORTC, &PORTD};
/* Input Register Array (Read only) */
static const volatile uint8* PIN_REG[NUM_OF_PORTS] = {&PINA, &PINB, &PINC, &PIND};


/*
 * Description :
 * Setup the direction of the required pin input/output.
 * If the input port number or pin number are not correct, The function will not handle the request.
 */
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PinDirectionType direction)
{
	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
	 * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 * In this case the input is not valid port/pin number
	 */
	if((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS))
	{
		/* Do Nothing */
	}
	else
	{
		/* Setup the pin direction as provided in the function argument */
		if(PIN_OUTPUT == direction){
			SET_BIT(*DDR_REG[port_num],pin_num);
		}
		else{
			CLEAR_BIT(*DDR_REG[port_num],pin_num);
		}
	}
}

/*
 * Description :
 * Write the value Logic High or Logic Low on the required pin.
 * If the input port number or pin number are not correct, The function will not handle the request.
 * If the pin is input, this function will enable/disable the internal pull-up resistor.
 */
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value)
{

	if((port_num >= NUM_OF_PORTS) || (pin_num >= NUM_OF_PINS_PER_PORT)){
		/* Do Nothing */
	}

	/* SET THE BIT or CLEAR THE BIT according to the value given to the function */
	else{
		if(LOGIC_HIGH == value){
			SET_BIT(*PORT_REG[port_num],pin_num);
		}

		else{
			CLEAR_BIT(*PORT_REG[port_num],pin_num);
		}
	}
}

/*
 * Description :
 * Read and return the value for the required pin, it should be Logic High or Logic Low.
 * If the input port number or pin number are not correct, The function will return Logic Low.
 */
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num)
{

	uint8 PIN_VALUE = LOGIC_LOW;
	/* if the Pin Number is invalid return Logic Low */
	if((port_num >= NUM_OF_PORTS) || (pin_num >= NUM_OF_PINS_PER_PORT)){
		return PIN_VALUE;
	}

	/* Check if the bit corresponding to pin_num is set in the selected port */
	if(BIT_IS_SET(*PIN_REG[port_num],pin_num)){
		PIN_VALUE = LOGIC_HIGH;
	}

	/* Finally return the Pin Value */
	return PIN_VALUE;
}

/*
 * Description :
 * Setup the direction of the required port all pins input/output.
 * If the direction value is PORT_INPUT all pins in this port should be input pins.
 * If the direction value is PORT_OUTPUT all pins in this port should be output pins.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_setupPortDirection(uint8 port_num, GPIO_PortDirectionType direction)
{

	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 * In this case the input is not valid port number
	 */
	if(port_num >= NUM_OF_PORTS)
	{
		/* Do Nothing */
	}
	else
	{
		/* Put the value of direction in the corresponding Port Number */
		*DDR_REG[port_num] = direction;
	}
}

/*
 * Description :
 * Write the value on the required port.
 * If any pin in the port is output pin the value will be written.
 * If any pin in the port is input pin this will activate/deactivate the internal pull-up resistor.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_writePort(uint8 port_num, uint8 value)
{
	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 * In this case the input is not valid port number
	 */
	if(port_num >= NUM_OF_PORTS)
	{
		/* Do Nothing */
	}
	else
	{
		/* Setup the port direction as required */
		*PORT_REG[port_num] = value;
	}
}

/*
 * Description :
 * Read and return the value of the required port.
 * If the input port number is not correct, The function will return ZERO value.
 */
uint8 GPIO_readPort(uint8 port_num)
{
	uint8 PIN_VALUE = LOGIC_LOW;
	if(port_num >= NUM_OF_PORTS){
		/* Do Nothing */
	}
	else{
		PIN_VALUE = *PIN_REG[port_num];
	}
	return PIN_VALUE;
}
