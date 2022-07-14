/*
 ============================================================================
 Name        : MC2_Function.c
 Data        : Jun 29, 2022
 Author      : The Bayoumi
 Description : 
 ============================================================================
 */

#include "MC2_Function.h"

/*
 ===============================================================================
 =                             Global Variables                                =
 ===============================================================================
 */

uint16 g_tick=0;	/* DC Motor g_tick */
uint16 g_ticks=0;	/* Buzzer g_tick */
uint8 Buzzer_Timer_Flag = 0;	/*Buzzer Flag*/
uint8 DCMotor_Timer_Flags = 0;	/*DC Motor Flag*/

/*
 ===============================================================================
 =                             Functions Definition                            =
 ===============================================================================
 */

/*
 * Description: Function for The DC Motor processing timer
 * 1 : Increment g_ticks
 * 2 : if the g_ticks incremented up to 459(15 Seconds), Change The Flag value to 1
 * 3 : if the g_ticks incremented up to 643(18 Seconds), Change The Flag value to 2
 * 4 : if the g_ticks incremented up to 1102(33 Seconds), Change The Flag value to 0
 */

void DCMotor_Processing (void)
{

	g_ticks++;
	if (g_ticks == NUMBER_OF_OverFlow_DcMotorStart_PER_15_SECOND)
	{
		DCMotor_Timer_Flags =1;
	}

	if (g_ticks == NUMBER_OF_OverFlow_DcMotorOverallTime_PER_18_SECOND)
	{
		DCMotor_Timer_Flags =2;
	}

	if (g_ticks == NUMBER_OF_OverFlow_DcMotorOverallTime_PER_33_SECOND)
	{
		DCMotor_Timer_Flags = 0;
		g_ticks = 0;
	}
}

/*
 * Description: Function for The Buzzer processing timer
 * 1 : Increment g_tick
 * 2 : if the g_tick incremented up to 1838(60 Seconds), Change The Flag value to 1
 */

void Buzzer_Processing_Time (void)
{
	g_tick++;

	if (g_tick == NUMBER_OF_OverFlow_BuzzerOverallTime_PER_60_SECOND)
	{
		Buzzer_Timer_Flag =1;

	}
	else if (g_tick > NUMBER_OF_OverFlow_BuzzerOverallTime_PER_60_SECOND)
	{
		Buzzer_Timer_Flag =0;
		g_tick = 0;
	}
}

/*
 * Description: Function to Store The Password into EEPROM
 */

void StorePassword (uint8 *StorePass_Ptr)
{
	uint8 i;

	for (i=0 ; i<ArrSize ; i++ )
	{
		EEPROM_writeByte( i , StorePass_Ptr[i]);
		_delay_ms(10);
	}
}

/*
 * Description: Function to Read The Password from EEPROM
 */

void ReadPassword (uint8 *StorePass_Ptr)
{
	uint8 i;

	for (i=0 ; i<ArrSize ; i++ )
	{
		EEPROM_readByte( i, &StorePass_Ptr[i]);
	}
}

/*
 * Description: Function to Compare & Store Passwords
 * 1 : Compare between the passwords
 * 2 : If Password is Correct
 * 2 : Choose one of Two Password
 * 3 : Call StorePassword Function to Store The Chosen Password in The EEPROM
 */

uint8 PasswordCompareStore (uint8  PasswordSize , uint8 * Password1 , uint8 * Password2 )
{
	uint8 i = 0;

	for ( i=0; i<ArrSize ; i++ )
	{
		if (Password1 [i] != Password2[i])
		{
			return NewPasswordNotMatch;
		}
	}
	StorePassword( Password1);

	return NewPasswordMatch;
}

/*
 * Description: Pointer to Function ReceiveNewPassword
 */

void (*ReceiveNewPassword_Ptr)()=&ReceiveNewPassword;

/*
 * Description: Function to Receive New Password.
 * 1 : Waiting to Receive The First Password From HMI Microcontroller.
 * 2 : Waiting to Receive The Second Password From HMI Microcontroller.
 * 3 : Send The Two Passwords to PasswordCompareStore Function.
 * 4 : If Password Matched, Send to HMI Microcontroller value indicates that, Go to next step.
 * 5 : If Password Didn't Match, Send to HMI Microcontroller value indicates that.
 * 6 : Repeat.
 */

void ReceiveNewPassword (void)
{
	uint8 i,ReceivePasswordConfirm=0;
	uint8 Password1[ArrSize];
	uint8 Password2[ArrSize];

	for (i= 0 ; i<ArrSize ; i++)
	{
		Password1 [i] = UART_recieveByte();
	}

	for (i= 0 ; i<ArrSize ; i++)
	{
		Password2[i] = UART_recieveByte();
	}

	ReceivePasswordConfirm = PasswordCompareStore(ArrSize,Password1,Password2);

	if (ReceivePasswordConfirm == NewPasswordMatch)
	{
		UART_sendByte(NewPasswordMatch);
	}
	else if (ReceivePasswordConfirm == NewPasswordNotMatch)
	{
		UART_sendByte(NewPasswordNotMatch);

		return ReceiveNewPassword_Ptr();
	}

}

/*
 * Description: Function to Compare Between Two Passwords, Entered Password & Saved Password
 * 1 : Load The Stored Password from the EEPROM
 * 2 : Compare The Two Password
 * 3 : If the Password Match, Return value to indicate that the Password is Correct
 * 4 : If the Password didn't Match, Return value to indicate that the Password is Wrong
 * 5 : If the Password didn't Match more than 3 times, Return value to Start the Buzzer
 */

uint8 PasswordCompare (uint8  PasswordSize , uint8 * Password1 )
{
	uint8 i;
	static uint8 counter=0;
	uint8 SavedPassword[ArrSize];

	ReadPassword(SavedPassword);

	for ( i=0; i<ArrSize ; i++ )
	{
		if (SavedPassword[i] != Password1[i])
		{
			counter++;
			if (counter ==3)
			{
				counter =0;

				return AlarmCheck;
			}
			return NotMatchedPassword;
		}
	}
	return MatchedPassword;
}

/*
 * Description: Function to Receive The Password
 * 1 : Waiting to Receive The Password From HMI Microcontroller
 * 2 : Send The Password to PasswordCompare Function
 * 3 : Returns a value that indicates weather the Password is Correct or Wrong or Alarm
 */

uint8 ReceiveComparePassword (void)
{
	uint8 i;
	uint8 PasswordCheck[ArrSize];
	uint8 ErrorCounter = 0;

	for ( i=0; i<ArrSize ; i++ )
	{
		PasswordCheck[i] =UART_recieveByte();
	}
	ErrorCounter = PasswordCompare(ArrSize,PasswordCheck);

	return ErrorCounter;
}



/*
 * Description: Function that control the Initialization of the Buzzer & DC Motor
 * 1 : Receive value indicates whether the password is Correct From ReceiveComparePassword's Function
 * 2 : If Password is Correct, Rotate The DC Motor Clockwise for 15 Seconds, Stop DC Motor for 3Seconds, Rotate The DC Motor Counter Clockwise for 15 Seconds
 * 3 : if Password isn't Correct more than 3 time start the Buzzer for 60 Seconds
 */

void OperationControl (void)
{
	uint8 ReceivePasswordCheck;

	/*Configuration of Motor Timer
	   ***Timer type : Timer0***
		 * 1 : Set Timer0 in compare mode
		 * 2 : Prescaler = T_F_CPU/1024
		 * 3 : Normal port operation, OC1A/OC1B disconnected.
		 * 4 : In case of Timer1, Choose Channel A or B
		 * 5 : Initial value = 0
		 * 6 : Final value = 255
		*/
	Timer_Config Motor_timer_configc = { CTC, T_F_CPU_1024, NormalMatchMode, 'A', 0, 255};

	/*Configuration of Buzzer Timer
	   ***Timer type : Timer1***
		 * 1 : Timer1 mode => Compare mode
		 * 2 : Prescaler = T_F_CPU/256
		 * 3 : Set OC1A/OC1B on compare match (Set output to high level)
		 * 4 : Select Channel A
		 * 5 : Set initial timer value to zero
		 * 6 : Compare value = 62500
	*/
	Timer_Config Buzzer_timer_config = { CTC, T_F_CPU_256, SetCompareMatch, 'A', 0, 62500};

	sei();		/*Enable Interrupts*/

	ReceivePasswordCheck=ReceiveComparePassword();

	if(ReceivePasswordCheck == MatchedPassword)
	{
		UART_sendByte(ReceivePasswordCheck);

		/*DC Motor Timer0 Call back*/
		Timer0_setCallBack(DCMotor_Processing);

		/*
		 * Timer Type : Timer0
		 * DC Motor Timer0 initialization
		 */
		Timer_Init(Timer0,&Motor_timer_configc);

		DcMotor_Rotate(Motor_CW);
		while (DCMotor_Timer_Flags == 0);

		DcMotor_Rotate(Motor_STOP);
		while (DCMotor_Timer_Flags == 1);

		DcMotor_Rotate(Motor_C_CW);
		while (DCMotor_Timer_Flags == 2);

		DcMotor_Rotate(Motor_STOP);

		/*DC Motor Timer0 deactivate*/
		Timer_DeInit(Timer0);

		return;
	}
	else if (ReceivePasswordCheck == NotMatchedPassword)
	{
		UART_sendByte(ReceivePasswordCheck);

		return;
	}
	else if (ReceivePasswordCheck== AlarmCheck)
	{
		UART_sendByte(ReceivePasswordCheck);

		/*Buzzer Timer1 Call back*/
		Timer1_setCallBack(Buzzer_Processing_Time);

		/*
		 * Timer Type : Timer1
		 * Buzzer Timer1 initialization
		 */
		Timer_Init(Timer1,&Buzzer_timer_config);

		Buzzer_Start();
		while ( Buzzer_Timer_Flag != 1 );
		Buzzer_Stop();

		/*Buzzer Timer1 deactivate*/
		Timer_DeInit(Timer1);
		Buzzer_Timer_Flag =0;
	}
	cli();		/*Disable Interrupts*/
}

/*
 * Description: Function to change the password form the Main menu
 * 1: Receive The User Entered Password
 * 2: Check whether the Entered Password is Correct or not
 * 3: if Password is Correct, Create new Password
 * 4: if Password isn't Correct, Do nothing
 */

void ChangePasswordOperation (void)
{
	uint8 ChangePassowrdCheck ;


	ChangePassowrdCheck = ReceiveComparePassword();
	if (ChangePassowrdCheck ==MatchedPassword)
	{
		UART_sendByte(ChangePassowrdCheck);
		ReceiveNewPassword();

	}
	else if (ChangePassowrdCheck == NotMatchedPassword)
	{
		UART_sendByte(ChangePassowrdCheck);

		return;
	}
	else if (ChangePassowrdCheck == AlarmCheck)
	{
		UART_sendByte(ChangePassowrdCheck);
		return;
	}
}

/*
 * Description: Function to Check The Menu State
 * 1 : Waiting for the main menu state from HMI Microcontroller
 * 2 : if MenuDoorStatus was sent, Call OperationControl Function
 * 3 : if MenuChangingPassword was sent, Call ChangePasswordOperation Function
 */


void MenuOperationControl (void)
{
	uint8 MainMenuState=0;

	MainMenuState = UART_recieveByte();

	if (MainMenuState == MenuDoorStatus)
	{
		OperationControl();


	}
	else if (MainMenuState == MenuChangingPassword)
	{
		ChangePasswordOperation();

	}
}
