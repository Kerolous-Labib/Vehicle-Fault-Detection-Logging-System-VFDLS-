/*
 * pwm.c
 *
 *  Created on: Sep 26, 2025
 *  Author: Kerolous-Labib
 */

#include "pwm.h"


/*
 * Description:
 * Configures and starts Timer0 in Fast PWM mode to generate a PWM signal.
 *
 * Variables:
 * - duty_cycle: The duty cycle value (0–255) to be loaded into OCR0.
 *               - 0   → 0% duty cycle (always LOW)
 *               - 255 → 100% duty cycle (always HIGH)
 *
 * Process:
 * - Sets OC0 (PORTB, PIN3) as output.
 * - Configures Timer0 in Fast PWM mode (WGM00, WGM01 = 1).
 * - Enables non-inverting mode (COM01 = 1 → PWM output clears on compare match,
 *   sets at bottom).
 * - Sets prescaler to 1024 (CS00 = 1, CS02 = 1).
 * - Loads duty_cycle into OCR0.
 * - Enables Output Compare Match Interrupt (OCIE0).
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	/* Configure OC0 (PB3) as output pin for PWM signal */
	GPIO_setupPinDirection(PORTB_ID, PIN3, PIN_OUTPUT);

	/* Configure Timer0:
	 - Fast PWM mode (WGM00=1, WGM01=1)
	 - Non-inverting mode (COM01=1)
	 - Prescaler = 1024 (CS00=1, CS02=1)
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<CS00) | (1<<CS02) | (1<<COM01);

	/* Set duty cycle (0–255) */
	OCR0 = duty_cycle;

	/* Enable Output Compare Match Interrupt for Timer0 */
	TIMSK |= (1<<OCIE0);
}


