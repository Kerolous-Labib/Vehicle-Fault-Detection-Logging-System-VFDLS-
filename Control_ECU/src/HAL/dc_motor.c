/*
 * dc_motor.c
 *
 *  Created on: Sep 28, 2025
 *  Author: Kerolous-Labib
 */

#include "dc_motor.h"
#include "pwm.h"

/*
 * Description:
 * Initializes both DC motors (MOTOR1 & MOTOR2).
 * - Configures all H-Bridge control pins as outputs.
 * - Sets both motors to STOP initially.
 * - Starts PWM with 0% duty cycle for speed control.
 */
void DcMotor_Init(MOTOR_typeConfig *ptr)
{

	/* ===== Motor Pins ===== */
	GPIO_setupPinDirection(ptr->PORT_ID, ptr->IN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(ptr->PORT_ID, ptr->IN2_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(ptr->PORT_ID, ptr->EN_ID, PIN_OUTPUT);


	/* Stop both motors */
	GPIO_writePin(ptr->PORT_ID, ptr->IN1_ID, LOGIC_LOW);
	GPIO_writePin(ptr->PORT_ID, ptr->IN2_ID, LOGIC_LOW);

	/* Start PWM with 0% duty (motor stopped) */
	PWM_Timer0_Start(0);
}

/*
 * Description:
 * Controls direction and speed of selected motor.
 */
void DcMotor_Rotate(MOTOR_typeConfig *ptr, DcMotor_State state, uint8 speed)
{
	uint8 pwmValue = (speed * PWM_MAX) / MOTOR_MAX;

		switch(state)
		{
		case STOP:
			GPIO_writePin(ptr->PORT_ID, ptr->IN1_ID, LOGIC_LOW);
			GPIO_writePin(ptr->PORT_ID, ptr->IN2_ID, LOGIC_LOW);
			break;

		case CW:
			GPIO_writePin(ptr->PORT_ID, ptr->IN1_ID, LOGIC_LOW);
			GPIO_writePin(ptr->PORT_ID, ptr->IN2_ID, LOGIC_HIGH);
			break;

		case A_CW:
			GPIO_writePin(ptr->PORT_ID, ptr->IN1_ID, LOGIC_HIGH);
			GPIO_writePin(ptr->PORT_ID, ptr->IN2_ID, LOGIC_LOW);
			break;
		}


	/* Apply PWM duty cycle to enable pins (speed control) */
	PWM_Timer0_Start(pwmValue);
}
