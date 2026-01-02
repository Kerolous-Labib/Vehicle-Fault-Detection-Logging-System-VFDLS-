/*
 * dc_motor.h
 *
 *  Created on: Sep 28, 2025
 *      Author: Kerolous-Labib
 */

#ifndef SRC_DC_MOTOR_H_
#define SRC_DC_MOTOR_H_

#include "gpio.h"
#include "std_types.h"

/*******************************************************************************
 *                                Data Types                                   *
 *******************************************************************************/
typedef enum {
	CW, A_CW, STOP
} DcMotor_State;

typedef struct {
	uint8 PORT_ID;
	uint8 IN1_ID;
	uint8 IN2_ID;
	uint8 EN_ID;
}MOTOR_typeConfig;

extern MOTOR_typeConfig MOTOR1_typeconfig;
extern MOTOR_typeConfig MOTOR2_typeconfig;

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


#define PWM_MAX               255
#define MOTOR_MAX             100

/*******************************************************************************
 *                              Function Prototypes                            *
 *******************************************************************************/

/*
 * Description:
 * Initializes both DC motors.
 *  - Configures H-Bridge pins (IN1, IN2, EN) for both motors as outputs.
 *  - Stops both motors initially.
 */
void DcMotor_Init(MOTOR_typeConfig *ptr);

/*
 * Description:
 * Controls the direction and speed of the selected motor.
 * Inputs:
 *  - Number: MOTOR1 or MOTOR2.
 *  - state:  STOP, CW, A_CW.
 *  - speed:  0–100 (%), converted internally to PWM duty.
 */
void DcMotor_Rotate(MOTOR_typeConfig *ptr, DcMotor_State state, uint8 speed);

#endif /* SRC_DC_MOTOR_H_ */
