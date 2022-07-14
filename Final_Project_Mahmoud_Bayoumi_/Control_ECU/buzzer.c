/*
 * buzzer.c
 *
 *  Created on: Jun 23, 2022
 *      Author: The Bayoumi
 */
#include "gpio.h"

void Buzzer_Init (void)
{
	GPIO_setupPinDirection(PORTC_ID, PIN2_ID, PIN_OUTPUT);
	return;
}

void Buzzer_Start (void)
{
	GPIO_writePin(PORTC_ID, PIN2_ID, LOGIC_HIGH);

	return;
}

void Buzzer_Stop (void)
{
	GPIO_writePin(PORTC_ID, PIN2_ID, LOGIC_LOW);

	return;
}
