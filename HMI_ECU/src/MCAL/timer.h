/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the TIMER AVR driver
 *
 * Author: Kerolous Labib
 *
 *******************************************************************************/

#ifndef SRC_MCAL_TIMER_H_
#define SRC_MCAL_TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/* Timer identifiers */
typedef enum{
	TIMER0_ID,
	TIMER1_ID,
	TIMER2_ID
}Timer_ID_Type;

/* Timer modes */
typedef enum{
	TIMER_OVF,
	TIMER_COMP,
	TIMER_PWM,
}Timer_ModeType;

/* Clock / prescaler selection values map to CSn2:0 bit patterns (0..7) */
typedef enum
{
    TIMER_NO_CLOCK,        			/* Timer stopped */
    TIMER_NO_PRESCALER,    			/* No prescaling (CPU clock directly) */
    TIMER_PRESCALER_8,     			/* Clock divided by 8 */
    TIMER_PRESCALER_64,   			/* Clock divided by 64 */
    TIMER_PRESCALER_256,   			/* Clock divided by 256 */
    TIMER_PRESCALER_1024,  			/* Clock divided by 1024 */
    TIMER_EXTERNAL_FALLING, 		/* External clock source on falling edge */
    TIMER_EXTERNAL_RISING   		/* External clock source on rising edge */
} Timer_ClockType;

/* Configuration structure
 * - Use the fields appropriate to the chosen timer & mode.
 *
 */
typedef struct
{
	uint16 timer_InitialValue;
	uint16 timer_compare_MatchValue;     /*it will be used in compare mode only*/
	uint16 timer_pwm_TopVlue;			/* it will be used in PWM mode only */
	Timer_ID_Type  timer_ID;
	Timer_ClockType timer_clock;
	Timer_ModeType  timer_mode;
}Timer_ConfigType;

extern Timer_ConfigType Timer_Config;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/


/*
 * Description:
 * 	Function to initialize the Timer driver
 *
 * Inputs:
 * 	pointer to the configuration structure with type Timer_ConfigType.
 *
 * Return: None
 */
void TIMER_init(const Timer_ConfigType * Config_Ptr);


/*
 * Description:
 * 	Function to disable the Timer via Timer_ID.
 *
 * Inputs:
 * 	Timer_ID.
 *
 * Return: None
 */
void TIMER_deInit(Timer_ID_Type timer_ID);

/*
 * Description:
 * 	Function to set the Call Back function address to the required Timer.
 *
 * Inputs:
 * 	pointer to Call Back function and Timer Id you want to set The Callback to it.
 *
 * Return: None
 */
void TIMER_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );

#endif /* SRC_MCAL_TIMER_H_ */
