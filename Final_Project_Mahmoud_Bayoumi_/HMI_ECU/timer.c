/*
 * timer.c
 *
 *  Created on: Jun 21, 2022
 *      Author: The Bayoumi
 */

#include "timer.h"

static volatile void (*g_callBackPtr0)(void) = NULL_PTR;

static volatile void (*g_callBackPtr1)(void) = NULL_PTR;

static volatile void (*g_callBackPtr2)(void) = NULL_PTR;

ISR (TIMER0_COMP_vect)

{
	if (g_callBackPtr0 != NULL_PTR)
	{
		(*g_callBackPtr0)();
	}
}

ISR (TIMER0_OVF_vect)

{
	if (g_callBackPtr0 != NULL_PTR)
	{
		(*g_callBackPtr0)();
	}
}

ISR (TIMER1_COMPA_vect)

{
	if (g_callBackPtr1 != NULL_PTR)
	{
		(*g_callBackPtr1)();
	}
}

ISR (TIMER1_COMPB_vect)

{
	if (g_callBackPtr1 != NULL_PTR)
	{
		(*g_callBackPtr1)();
	}
}

ISR (TIMER1_OVF_vect)

{
	if (g_callBackPtr1 != NULL_PTR)
	{
		(*g_callBackPtr1)();
	}
}

ISR (TIMER2_COMP_vect)

{
	if (g_callBackPtr2 != NULL_PTR)
	{
		(*g_callBackPtr2)();
	}
}

ISR (TIMER2_OVF_vect)

{
	if (g_callBackPtr2 != NULL_PTR)
	{
		(*g_callBackPtr2)();
	}
}

void Timer_Init (uint8 TimerNumber,const Timer_Config *config_Ptr )
{
	if (TimerNumber == Timer0)
	{
		TCNT0 = config_Ptr ->Init_Timer_Value;

		TCCR0 =  (TCCR0 & 0xF8) | (config_Ptr ->prescaler);

		SET_BIT( TCCR0, FOC0);


		if ((config_Ptr ->mode) == Normal)
		{
			CLEAR_BIT( TCCR0, WGM00);

			CLEAR_BIT( TCCR0, WGM01);

			SET_BIT (TIMSK , TOIE0);
		}
		else if ((config_Ptr ->mode) == CTC)
		{
			CLEAR_BIT( TCCR0, WGM00);

			SET_BIT( TCCR0, WGM01);

			OCR0 = config_Ptr ->Final_Timer_Value;

			TCCR0 = (TCCR0 & 0XCF) | ((config_Ptr->CompareMode)<<4);

			SET_BIT(TIMSK, OCIE0);
		}
	}
	else if (TimerNumber == Timer1)
	{
		SET_BIT ( TCCR1A, FOC1A );

		SET_BIT ( TCCR1B, FOC1B );

		TCNT1 = config_Ptr ->Init_Timer_Value;

		TCCR1B = (TCCR1B & 0XF8) | (config_Ptr ->prescaler);

		if ((config_Ptr ->mode) == Normal)
		{
			CLEAR_BIT ( TCCR1A, COM1B0 );

			CLEAR_BIT ( TCCR1A, COM1B1 );

			CLEAR_BIT ( TCCR1A, COM1A0 );

			CLEAR_BIT ( TCCR1A, COM1A1 );


			CLEAR_BIT ( TCCR1B, WGM10 );

			CLEAR_BIT ( TCCR1B, WGM11 );

			CLEAR_BIT ( TCCR1B, WGM12 );

			CLEAR_BIT ( TCCR1B, WGM13 );


			SET_BIT ( TIMSK, TOIE1 );
		}
		else if ((config_Ptr ->mode) == CTC)
		{
			TCCR1A = ( TCCR1A & 0xF8) | ((config_Ptr -> CompareMode)<<4);

			CLEAR_BIT ( TCCR1A, WGM10);

			CLEAR_BIT ( TCCR1A, WGM11);

			SET_BIT ( TCCR1B, WGM12);

			CLEAR_BIT ( TCCR1B, WGM13);

			if ((config_Ptr -> Channel) == 'A')
			{
				OCR1A = config_Ptr -> Final_Timer_Value;

				SET_BIT ( TIMSK, OCIE1A);
			}
			else if ((config_Ptr -> Channel) == 'B')
			{
				OCR1B = config_Ptr -> Final_Timer_Value;

				SET_BIT ( TIMSK, OCIE1B);
			}
		}
	}
	else if (TimerNumber == Timer2)
	{

			TCNT2 = config_Ptr ->Init_Timer_Value;

			TCCR2 =  (TCCR2 & 0xF8) | (config_Ptr ->prescaler);

			SET_BIT( TCCR2, FOC2);

			if ((config_Ptr ->mode) == Normal)
			{
				CLEAR_BIT( TCCR2, WGM20);

				CLEAR_BIT( TCCR2, WGM21);

				SET_BIT ( TIMSK, TOIE0);
			}
			else if ((config_Ptr ->mode) == CTC)
			{
				CLEAR_BIT( TCCR2, WGM20);

				SET_BIT( TCCR2, WGM21);

				OCR2 = config_Ptr ->Final_Timer_Value;

				TCCR2 = (TCCR2 & 0XCF) | ((config_Ptr->CompareMode)<<4);

				SET_BIT( TIMSK, OCIE2);
			}
	}
}

void Timer0_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr0 = a_ptr;
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr1 = a_ptr;
}

void Timer2_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr2 = a_ptr;
}

void Timer_DeInit (uint8 TimerType)
{
	if (TimerType == Timer0)
	{
		TCNT0 = 0;

		TCCR0 = 0;

		CLEAR_BIT (TIMSK , TOIE0);
	}
	else if (TimerType == Timer1)
	{
		TCCR1A = 0;
		TCCR1B = 0;
		OCR1A = 0;
		TIMSK = 0;
	}
	else if (TimerType == Timer2)
	{
		TCNT2 = 0;

		TCCR2 = 0;

		CLEAR_BIT (TIMSK , TOIE2);
	}
}
