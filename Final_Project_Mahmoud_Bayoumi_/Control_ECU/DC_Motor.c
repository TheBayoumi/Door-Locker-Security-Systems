/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DC_Mototr.c
 *
 * Description: Source file for the ATmega16 Motor driver
 *
 * Author: The Bayoumi
 *
 *******************************************************************************/

#include "DC_Motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the DC Motor
 * 1. Setup the DC Motor pins directions by use the GPIO driver.
 * 2. Setup the DC Motor state ( STOP - CLOCKWISE - COUNTER CLOCKWISE)
 */

void DcMotor_init(void)
{
	/* Configure the direction for IN1 and IN2 pins as output pins */
	GPIO_setupPinDirection(DcMotor_PORT_ID,DcMotor_IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DcMotor_PORT_ID,DcMotor_IN2_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN2_PIN_ID,LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for rotate the DC Motor CLOCKWISE or COUNTER CLOCKWISE or STOP the motor based on the state input state value.
 */

void DcMotor_Rotate(uint8 Direction)
{
	if(Direction == Motor_STOP){
		GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN2_PIN_ID,LOGIC_LOW);
	}
	else if (Direction == Motor_CW)
	{
		GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN2_PIN_ID,LOGIC_LOW);
	}
	else if (Direction == Motor_C_CW)
		{
		GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DcMotor_PORT_ID,DcMotor_IN2_PIN_ID,LOGIC_HIGH);
		}
}

