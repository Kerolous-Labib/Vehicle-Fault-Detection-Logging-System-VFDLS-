/*
 * adc.h
 *
 *  Created on: Sep 25, 2025
 *      Author: Kerolous-Labib
 */

#ifndef SRC_ADC_H_
#define SRC_ADC_H_

/*******************************************************************************
 *                                Libraries                                    *
 *******************************************************************************/
#include "gpio.h"
#include <avr/io.h>
#include "common_macros.h"


/*******************************************************************************
 *                                Data Types                                    *
 *******************************************************************************/

/*
 * Enum for ADC reference voltage selection.
 * Options correspond to REFS1,REFS0 bits in ADMUX register.
 *
 * External voltage at AREF pin, internal Vref turned off
 * AVCC with external capacitor at AREF pin
 * Internal 2.56V reference with external capacitor at AREF pin
 */
typedef enum{
	ADC_REF_AREF,ADC_REF_AVCC,ADC_REF_RESERVED,ADC_REF_INTERNAL
}ADC_Reference;

/*
 * Enum for ADC prescaler values.
 * These values divide the CPU clock to generate the ADC clock.
 * The ADC clock must be between 50 kHz and 200 kHz for accurate results.
 */
typedef enum{
	ADC_PRESCALER_2 = 1,			// F_CPU / 2
	ADC_PRESCALER_4,				// F_CPU / 4
	ADC_PRESCALER_8,				// F_CPU / 8
	ADC_PRESCALER_16,				// F_CPU / 16
	ADC_PRESCALER_32,				// F_CPU / 32
	ADC_PRESCALER_64,				// F_CPU / 64
	ADC_PRESCALER_128				// F_CPU / 128
}ADC_Prescaler;


/*
 * Enum for ADC channels (ADC0–ADC7).
 */
typedef enum {
    ADC_0, ADC_1, ADC_2, ADC_3, ADC_4, ADC_5, ADC_6, ADC_7
} ADC_Channel;


/*
 * Structure for ADC configuration.
 * - reference : select voltage reference source
 * - prescaler : select clock prescaler
 */
typedef struct{
	ADC_Reference reference;
	ADC_Prescaler prescaler;
}ADC_typeConfig;


/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/

/*
 * Global configuration structure for the ADC driver.
 */

extern ADC_typeConfig ADC_config;



/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/


/*
 * Description:
 * Initializes the ADC with the given configuration.
 */
void ADC_init(ADC_typeConfig *ptr);

/*
 * Description:
 * Reads an analog value from the specified ADC channel.
 */
uint16 ADC_readChannel(uint8 channel_num);

#endif /* SRC_ADC_H_ */
