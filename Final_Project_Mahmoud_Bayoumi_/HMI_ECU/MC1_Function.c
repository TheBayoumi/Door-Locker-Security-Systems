/*
 ============================================================================
 Name        : MC1_Function.c
 Data        : Jun 29, 2022
 Author      : The Bayoumi
 Description : 
 ============================================================================
 */

#include "MC1_Function.h"


/*
 ===============================================================================
 =                                Global Variables                             =
 ===============================================================================
 */

uint16 g_tick=0;	/* Dc Motor g_tick */
uint16 g_ticks=0;	/* Buzzer g_tick */
uint8 DCMotor_Timer_Flag = 0;
uint8 Buzzer_Timer_Flag = 0;

/*
 ===============================================================================
 =                                Function Definition                          =
 ===============================================================================
 */

/*
 * Description: Function for The DC Motor processing timer
 * 1 : Increment g_ticks
 * 2 : if the g_ticks incremented up to 456(15 Seconds), Change The Flag value to 1
 * 3 : if the g_ticks incremented up to 550(18 Seconds), Change The Flag value to 2
 * 4 : if the g_ticks incremented up to 1006(33 Seconds), Change The Flag value to 0
 */

void DCMotor_Start_Processing (void)
{
	g_tick++;
	if (g_tick == NUMBER_OF_OverFlow_DcMotorStart_PER_15_SECOND)
	{
		DCMotor_Timer_Flag =1;

	}
	if (g_tick == NUMBER_OF_OverFlow_DcMotorOverallTime_PER_18_SECOND)
	{
		DCMotor_Timer_Flag =2;
	}

	if (g_tick == NUMBER_OF_OverFlow_DcMotorOverallTime_PER_33_SECOND)

	{
		DCMotor_Timer_Flag = 0;
		g_tick = 0;
	}
}

/*
 * Description: Function for The Buzzer processing timer
 * 1 : Increment g_tick
 * 2 : The g_tick increments up to 30 for(60 Seconds), Change The Flag value to 1
 */

void Buzzer_Processing_Time (void)
{
	g_ticks++;
	if (g_ticks == NUMBER_OF_OverFlow_BuzzerOverallTime_PER_60_SECOND)
	{
		Buzzer_Timer_Flag =1;
	}
	else if (g_ticks > NUMBER_OF_OverFlow_BuzzerOverallTime_PER_60_SECOND)
	{
		Buzzer_Timer_Flag =0;
		g_ticks = 0;
	}
}

/*
 * Description: Function to send Password to be checked stored in EEPROM
 */

void SendPassword (uint8 ArraySize, uint8 *Password)
{
	uint8 i;

	for ( i=0 ; i<ArraySize ; i++ )
	{
		UART_sendByte(Password[i]);
	}
}


/*
 * Description: Function to register valid keypad input values and send it
 */

uint8 ValidKeypadEntry (void)
{
	uint8 i;
	uint8 Password[ArrSize];
	volatile uint32 cnt;

	/*Filling password array with keypad entry value*/
	for(i=0; i<ArrSize; i++)
	{
		/*Get the Keypad input value and Store it in Password*/
		Password[i]= KEYPAD_getPressedKey();

		/*Delay for Keypad debouncing*/
		for(cnt=0 ; cnt<110000 ; cnt++);

		/*Keypad input value range*/
		if (( Password[i] >= 0 ) && ( Password[i] <=9 ))
		{
			LCD_displayStringRowColumn(1, i, "*");
		}
		else
		{
			/*In case the keypad input value is out range*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Invalid Number");
			for(cnt=0 ; cnt<110000 ; cnt++);

			/*Reset the counting of Password array*/
			i=-1;
			return 0;
		}
	}

	/* Use SendPassword Function to send the password for check*/
	SendPassword(ArrSize, Password);
	return 1;
}

/*
 * Description: Function to Display The New Password with The Keypad Entry
 */

void DisplayNewPassword (void)
{
	uint8 counter=0 ;
	uint8 Password1Check;
	uint8 Password2Check;

	if (counter == 0 )
	{

		do
		{

			LCD_clearScreen();
			/*Display New Password in row 0, Column 0*/
			LCD_displayStringRowColumn(0, 0, "New Password");
			/*To Check the return value and exit is the check completed*/
			Password1Check = ValidKeypadEntry();

		}while ( Password1Check == 0 );
		/*in order to exit the loop */
		counter ++;
	}

	if (counter == 1 )
	{
		do
		{

			LCD_clearScreen();
			/*Display Confirm Password in row 0, Column 0*/
			LCD_displayStringRowColumn(0, 0, "Confirm Password");

			/*To Check the return value and exit if the check completed*/
			Password2Check = ValidKeypadEntry();

		}while ( Password2Check == 0 );
	}
}

/*
 * Description: Function to Display Enter Password
 */

void DisplayEnterPassword (void)
{
	uint8 PasswordCheck;

	do
	{

		LCD_clearScreen();

		/*Display Enter Password in row 0, Column 0*/
		LCD_displayStringRowColumn(0, 0, "Enter Password");

		/*To Check the return value and exit if the check completed*/
		PasswordCheck = ValidKeypadEntry();

	}while ( PasswordCheck == 0 );
}

/*
 * Description: Pointer to Function CreateNewPassword
 */

void (*DisplayNewPasswordCheck_Ptr)()=&DisplayNewPasswordCheck;

/*
 * Description: Function to display New Password options
 */

void DisplayNewPasswordCheck (void)

{
	uint8 ConfirmPassword=0;
	volatile uint32 cnt;


	LCD_displayStringRowColumn(0, 0, "New Password");

	DisplayNewPassword();
	/*Waiting for the compare check on password from Control_ECU*/
	ConfirmPassword = UART_recieveByte();

	if (ConfirmPassword == NewPasswordMatch)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Password Confirmed");
		for(cnt=0 ; cnt<110000 ; cnt++);
	}
	else if (ConfirmPassword == NewPasswordNotMatch)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Not Confirmed");
		for(cnt=0 ; cnt<110000 ; cnt++);
		LCD_clearScreen();
		/*in case of password is wrong return to DisplayNewPasswordCheck*/
		return DisplayNewPasswordCheck_Ptr();
	}
}

/*
 * Description: Function to display Enter Password options
 */

uint8 DisplayEnterPasswordCheck (void)
{
	uint8  Check ;

	volatile uint32 cnt;

	DisplayEnterPassword();

	/*Waiting for the compare check on password from Control_ECU*/
	Check=UART_recieveByte();

	LCD_clearScreen();

	if (Check== MatchedPassword)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Correct Password");
		for(cnt=0 ; cnt<110000 ; cnt++);
		LCD_clearScreen();

	}
	else if (Check == NotMatchedPassword)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Incorrect Password");
		for(cnt=0 ; cnt<110000 ; cnt++);
		LCD_clearScreen();
	}
	else if (Check == AlarmCheck )
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Incorrect Password");
		for(cnt=0 ; cnt<90000 ; cnt++);
		LCD_clearScreen();
	}
	return Check;
}

/*
 * Description : Function to control the actuation of the buzzer and DC motor
 */

void OpenDoorApplication (void)
{
	uint8 PasswordMainCheck;
	PasswordMainCheck = DisplayEnterPasswordCheck();

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

	/*Configuration of Motor Timer
	 ***Timer type : Timer0***
	 * 1 : Set Timer0 in compare mode
	 * 2 : Prescaler = T_F_CPU/1024
	 * 3 : Normal port operation, OC1A/OC1B disconnected.
	 * 4 : In case of Timer1, Choose Channel A or B
	 * 5 : Initial value = 0
	 * 6 : Final value = 255
	 */
	Timer_Config Motor_timer_config = { CTC, T_F_CPU_1024, NormalMatchMode, 'A', 0, 255};

	if (PasswordMainCheck == MatchedPassword )
	{
		cli();		/*Disable Interrupts*/
		sei();		/*Enable Interrupts*/
		LCD_clearScreen();

		/*DC Motor Timer0 Call back*/
		Timer0_setCallBack(DCMotor_Start_Processing);

		/*
		 * Timer Type : Timer0
		 * DC Motor Timer0 initialization
		 */
		Timer_Init(Timer0,&Motor_timer_config);


		LCD_displayStringRowColumn(0, 0, "Door is Opening");
		while (DCMotor_Timer_Flag == 0);
		LCD_clearScreen();

		LCD_displayStringRowColumn(0, 0, "Door is Opened");
		while (DCMotor_Timer_Flag == 1);
		LCD_clearScreen();

		LCD_displayStringRowColumn(0, 0, "Door is Closing");
		while (DCMotor_Timer_Flag == 2);
		LCD_clearScreen();

		/*DC Motor Timer0 deactivate*/
		Timer_DeInit(Timer0);
		cli();		/*Disable Interrupts*/
		return;
	}
	else if (PasswordMainCheck == NotMatchedPassword )
	{
		return;
	}
	else if (PasswordMainCheck == AlarmCheck )
	{
		cli();		/*Disable Interrupts*/
		sei();		/*Enable Interrupts*/

		/*Buzzer Timer1 Call back*/
		Timer1_setCallBack(Buzzer_Processing_Time);

		/*
		 * Timer Type : Timer1
		 * Buzzer Timer1 initialization
		 */
		Timer_Init(Timer1,&Buzzer_timer_config);

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "ALARM!!");
		while ( Buzzer_Timer_Flag != 1 );

		LCD_clearScreen();

		/*Buzzer Timer1 deactivate*/
		Timer_DeInit(Timer1);
		cli();		/*Disable Interrupts*/
	}
}

/*
 * Description : Function to control the Change of password instruction
 */

void ChnagePasswordApplication (void)
{
	uint8 PasswordNewCheck;

	PasswordNewCheck=DisplayEnterPasswordCheck();

	if (PasswordNewCheck == MatchedPassword)
	{
		DisplayNewPasswordCheck();
		LCD_displayStringRowColumn(0, 0, "Password Changed successfully");
	}
	else if (PasswordNewCheck == NotMatchedPassword )
	{
		return;
	}
	else if (PasswordNewCheck == AlarmCheck )
	{
		return;

	}
}

/*
 * Description: Function that control the main menu display options
 */

void MainMenuCheck (void)
{
	volatile uint32 cnt;
	uint8 status;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+:Open Door");
	LCD_displayStringRowColumn(1, 0, "-:ChangePassword");

	status = KEYPAD_getPressedKey();
	for(cnt=0 ; cnt<110000 ; cnt++);
	/*In case of Press +, Start the motor*/
	if (status == '+')
	{
		UART_sendByte(MenuDoorStatus);

		LCD_clearScreen();

		OpenDoorApplication();
	}
	/*In case of press -, Change the Password*/
	else if (status == '-')
	{
		LCD_clearScreen();
		UART_sendByte(MenuChangingPassword);
		ChnagePasswordApplication();
	}
	/*In case of press other key, Display Invalid data*/
	else
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Invalid Data");
		for(cnt=0 ; cnt<110000 ; cnt++);
	}
}
