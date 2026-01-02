/*
 * adc.c
 *
 *  Created on: Sep 25, 2025
 *  Author: Kerolous-Labib
 */

#include "adc.h"


void ADC_init(ADC_typeConfig *ptr)
{
    /* Set voltage reference */
    ADMUX = (ptr->reference << 6);

    /* Enable ADC + prescaler */
    ADCSRA = (1<<ADEN) | (ptr->prescaler);
}

uint16 ADC_readChannel(uint8 channel_num)
{
    uint16 ADC_value;

    /* Select channel (clear MUX bits first) */
    ADMUX = (ADMUX & 0xF0) | (channel_num & 0x0F);

    /* Start conversion */
    ADCSRA |= (1<<ADSC);

    /* Wait until conversion complete */
    while(BIT_IS_CLEAR(ADCSRA,ADIF));

    /* Clear interrupt flag by writing 1 */
    ADCSRA |= (1<<ADIF);

    /* Read ADC value */
    ADC_value = ADC;

    return ADC_value;
}
