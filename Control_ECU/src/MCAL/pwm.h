/*
 * pwm.h
 *
 *  Created on: Sep 26, 2025
 *      Author: Kerolous-Labib
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_


/*******************************************************************************
 *                                Libraries                                    *
 *******************************************************************************/
#include "gpio.h"
#include <avr/io.h>
#include "common_macros.h"

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

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
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* SRC_PWM_H_ */
