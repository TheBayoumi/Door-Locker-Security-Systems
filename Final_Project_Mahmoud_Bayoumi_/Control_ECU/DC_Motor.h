/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DC_Mototr.h
 *
 * Description: Header file for the ATmega16 Motor driver
 *
 * Author: The Bayoumi
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "gpio.h"
#include "common_macros.h"
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* IN1 & IN2 Pins to PIN0 & PIN1 of Port B. */

#define DcMotor_PORT_ID                 PORTD_ID
#define DcMotor_IN1_PIN_ID              PIN2_ID
#define DcMotor_IN2_PIN_ID              PIN3_ID

typedef enum
{
	Motor_STOP,Motor_CW,Motor_C_CW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to Initialize the DC Motor.
 */

void DcMotor_init(void);

/*
 * Description :
 * function responsible for rotate the DC Motor CLOCKWISE or COUNTER CLOCKWISE or STOP the motor based on the state input state value.
 */

void DcMotor_Rotate(uint8 Direction);

#endif /* DC_MOTOR_H_ */
