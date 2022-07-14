/*
 * timer.h
 *
 *  Created on: Jun 21, 2022
 *      Author: The Bayoumi
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "gpio.h"
#include "avr/io.h"
#include "std_types.h"
#include <avr/interrupt.h>
#include "common_macros.h"

#define Timer0 0
#define Timer1 1
#define Timer2 2

typedef enum {
	Normal ,PWM_Phase_Correct, CTC, Fast_PWM
}Timer_Mode;

typedef enum {
	NormalMatchMode, ToggleCompareMatch, ClearCompareMatch, SetCompareMatch
}Timer_Compare_Match_Mode;

typedef enum
{
	T_NO_CLOCK,T_F_CPU_CLOCK,T_F_CPU_8,T_F_CPU_64,T_F_CPU_256,T_F_CPU_1024
}Timer_Prescaler;

typedef struct {
	Timer_Mode mode;
	Timer_Prescaler prescaler;
	Timer_Compare_Match_Mode CompareMode;
	uint8 Channel;
	uint8 Init_Timer_Value;
	uint32 Final_Timer_Value;
}Timer_Config;

void Timer0_setCallBack(void(*a_ptr)(void));

void Timer1_setCallBack(void(*a_ptr)(void));

void Timer2_setCallBack(void(*a_ptr)(void));

void Timer_Init (uint8 TimerNumber,const Timer_Config *config_Ptr );

void Timer0_setCallBack(void(*a_ptr)(void));

void Timer1_setCallBack(void(*a_ptr)(void));

void Timer2_setCallBack(void(*a_ptr)(void));

void Timer_DeInit (uint8 TimerType);

#endif /* TIMER_H_ */
