 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the TIMER AVR driver
 *
 * Author: Kerolous Labib
 *
 *******************************************************************************/

#include "timer.h"
#include "gpio.h"
#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/* Global variables to hold the address of the call back function in the application */
static void (*g_callBackPtr_Timer0)(void) = NULL_PTR;
static void (*g_callBackPtr_Timer1)(void) = NULL_PTR;
static void (*g_callBackPtr_Timer2)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr_Timer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_Timer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr_Timer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr_Timer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr_Timer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr_Timer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*
 * Description:
 *  Function to initialize the Timer driver.
 *
 * Inputs:
 *  Pointer to the configuration structure with type Timer_ConfigType.
 *
 * Return:
 *  None
 */
void TIMER_init(const Timer_ConfigType * Config_Ptr)
{


    /**************************************************************
     *                       TIMER0
     **************************************************************/
    if (Config_Ptr->timer_ID == TIMER0_ID)
    {
        /* Overflow Mode */
        if (Config_Ptr->timer_mode == TIMER_OVF)
        {
            /* Normal mode (WGM00=0, WGM01=0),
             * Force Output Compare enabled
             */
            TCCR0 = (1 << FOC0) | (Config_Ptr->timer_clock & 0x07);
            TCNT0 = Config_Ptr->timer_InitialValue;   /* Initial counter value */
            TIMSK |= (1 << TOIE0);                    /* Enable overflow interrupt */
        }

        /* Compare Match Mode */
        else if (Config_Ptr->timer_mode == TIMER_COMP)
        {
            TCCR0 = (1 << FOC0) | (1 << WGM01) | (Config_Ptr->timer_clock & 0x07);
            OCR0 = Config_Ptr->timer_compare_MatchValue;		/* Set the Compare value */
            TIMSK |= (1 << OCIE0);                    			/* Enable compare interrupt */
        }

        /* PWM Mode */
        else if (Config_Ptr->timer_mode == TIMER_PWM)
        {
            /* OC0 (PB3) as output for PWM signal */
            GPIO_setupPinDirection(PORTB_ID, PIN3, PIN_OUTPUT);

            /* Fast PWM, non-inverted (COM00=0, COM01=1) */
            TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (Config_Ptr->timer_clock & 0x07);
            OCR0 = Config_Ptr->timer_compare_MatchValue;  /* Set duty cycle */
        }
    }

    /**************************************************************
     *                       TIMER1
     **************************************************************/
    else if (Config_Ptr->timer_ID == TIMER1_ID)
    {
        /* Overflow Mode */
        if (Config_Ptr->timer_mode == TIMER_OVF)
        {
            TCCR1A = (1 << FOC1A) | (1 << FOC1B);
            TCCR1B = (Config_Ptr->timer_clock & 0x07);
            TCNT1 = Config_Ptr->timer_InitialValue;
            TIMSK |= (1 << TOIE1);
        }

        /* Compare Match Mode */
        else if (Config_Ptr->timer_mode == TIMER_COMP)
        {
            TCCR1A = (1 << FOC1A) | (1 << FOC1B);
            TCCR1B = (1 << WGM12) | (Config_Ptr->timer_clock & 0x07);
            OCR1A = Config_Ptr->timer_compare_MatchValue;
            TIMSK |= (1 << OCIE1A);
        }

        /* PWM Mode (Fast PWM, ICR1 defines TOP, OCR1A defines duty) */
        else if (Config_Ptr->timer_mode == TIMER_PWM)
        {
            /* OC1A (PD5) as output for PWM signal */
            GPIO_setupPinDirection(PORTD_ID, PIN5, PIN_OUTPUT);

            /* Fast PWM, non-inverted output on OC1A */
            TCCR1A = (1 << COM1A1) | (1 << WGM11);
            TCCR1B = (1 << WGM12) | (1 << WGM13) | (Config_Ptr->timer_clock & 0x07);

            ICR1  = Config_Ptr->timer_pwm_TopVlue;               /* Set PWM period (TOP) */
            OCR1A = Config_Ptr->timer_compare_MatchValue;   		/* Set duty cycle */
        }
    }

    /**************************************************************
     *                       TIMER2
     **************************************************************/
    else if (Config_Ptr->timer_ID == TIMER2_ID)
    {
        /* Overflow Mode */
        if (Config_Ptr->timer_mode == TIMER_OVF)
        {
            TCCR2 = (1 << FOC2) | (Config_Ptr->timer_clock & 0x07);
            TCNT2 = Config_Ptr->timer_InitialValue;
            TIMSK |= (1 << TOIE2);
        }

        /* Compare Match Mode */
        else if (Config_Ptr->timer_mode == TIMER_COMP)
        {
            TCCR2 = (1 << FOC2) | (1 << WGM21) | (Config_Ptr->timer_clock & 0x07);
            OCR2 = Config_Ptr->timer_compare_MatchValue;
            TIMSK |= (1 << OCIE2);
        }

        /* PWM Mode */
        else if (Config_Ptr->timer_mode == TIMER_PWM)
        {
            /* OC2 (PD7) as output for PWM signal */
            GPIO_setupPinDirection(PORTD_ID, PIN7, PIN_OUTPUT);

            /* Fast PWM, non-inverted */
            TCCR2 = (1 << WGM20) | (1 << WGM21) | (1 << COM21) | (Config_Ptr->timer_clock & 0x07);
            OCR2 = Config_Ptr->timer_compare_MatchValue;  /* Set duty cycle */
        }
    }
}



/*
 * Description:
 *  Function to disable the Timer via Timer_ID.
 *
 * Inputs:
 *  Timer_ID.
 *
 * Return:
 *  None
 */
void TIMER_deInit(Timer_ID_Type timer_ID)
{
    if (timer_ID == TIMER0_ID)
    {
        TCCR0 = 0;
        TCNT0 = 0;
        OCR0  = 0;

        /* Disable Timer0 interrupts */
        TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));

        /* Clear any pending flags */
        TIFR  |= (1 << TOV0) | (1 << OCF0);

    	/* Reset the global pointer value */
        g_callBackPtr_Timer0 = NULL_PTR;

    }

    else if (timer_ID == TIMER1_ID)
    {
        TCCR1A = 0;
        TCCR1B = 0;
        TCNT1  = 0;
        OCR1A  = 0;
        ICR1   = 0;

        /* Disable Timer1 interrupts */
        TIMSK &= ~((1 << TOIE1) | (1 << OCIE1A) | (1 << OCIE1B) | (1 << TICIE1));

        /* Clear pending flags */
        TIFR  |= (1 << TOV1) | (1 << OCF1A) | (1 << OCF1B) | (1 << ICF1);

    	/* Reset the global pointer value */
        g_callBackPtr_Timer1 = NULL_PTR;

    }

    else if (timer_ID == TIMER2_ID)
    {
        TCCR2 = 0;
        TCNT2 = 0;
        OCR2  = 0;

        /* Disable Timer2 interrupts */
        TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));

        /* Clear pending flags */
        TIFR  |= (1 << TOV2) | (1 << OCF2);

    	/* Reset the global pointer value */
        g_callBackPtr_Timer2 = NULL_PTR;

    }
}


/*
 * Description:
 * 	Function to set the Call Back function address to the required Timer.
 *
 * Inputs:
 * 	pointer to Call Back function and Timer Id you want to set The Callback to it.
 *
 * Return: None
 */
void TIMER_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
{
	if(a_timer_ID == TIMER0_ID){
		g_callBackPtr_Timer0 = a_ptr;
	}

	else if(a_timer_ID == TIMER1_ID){
		g_callBackPtr_Timer1 = a_ptr;
	}

	else if(a_timer_ID == TIMER2_ID){
		g_callBackPtr_Timer2 = a_ptr;
	}

}
