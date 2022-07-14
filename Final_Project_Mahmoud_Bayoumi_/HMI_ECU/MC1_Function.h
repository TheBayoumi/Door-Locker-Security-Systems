   /*
 ============================================================================
 Name        : MC1_Function.h
 Data        : Jun 29, 2022
 Author      : The Bayoumi
 Description : 
 ============================================================================
 */
#ifndef MC1_FUNCTION_H_
#define MC1_FUNCTION_H_

#include "timer.h"
#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include <util/delay.h>

/*
 ===============================================================================
 =                                Definition                                   =
 ===============================================================================
 */

#define NewPasswordNotMatch 19
#define NewPasswordMatch 18
#define AlarmCheck 17
#define MatchedPassword 9
#define NotMatchedPassword 8
#define MenuDoorStatus 7
#define MenuChangingPassword 6

#define ArrSize 5
#define NUMBER_OF_OverFlow_DcMotorStart_PER_15_SECOND 456
#define NUMBER_OF_OverFlow_DcMotorOverallTime_PER_18_SECOND 550
#define NUMBER_OF_OverFlow_DcMotorOverallTime_PER_33_SECOND 1006
#define NUMBER_OF_OverFlow_BuzzerOverallTime_PER_60_SECOND 30

/*
 ===============================================================================
 =                             Functions Prototypes                            =
 ===============================================================================
 */

/*
 * Description: Function for The DC Motor processing timer
 * 1 : Increment g_ticks
 * 2 : if the g_ticks incremented up to 456(15 Seconds), Change The Flag value to 1
 * 3 : if the g_ticks incremented up to 550(18 Seconds), Change The Flag value to 2
 * 4 : if the g_ticks incremented up to 1006(33 Seconds), Change The Flag value to 0
 */


void DCMotor_Start_Processing (void);

/*
 * Description: Function for The Buzzer processing timer
 * 1 : Increment g_tick
 * 2 : The g_tick increments up to 30 for(60 Seconds), Change The Flag value to 1
 */

void Buzzer_Processing_Time (void);

/*
 * Description: Function to send Password to be checked stored in EEPROM
 */

void SendPassword (uint8 ArraySize, uint8 *Password);

/*
 * Description: Function to register valid keypad input values and send it
 */

uint8 ValidKeypadEntry (void);

/*
 * Description: Function to Display The New Password with The Keypad Entry
 */

void DisplayNewPassword (void);

/*
 * Description: Function to Display Enter Password
 */

void DisplayEnterPassword (void);

/*
 * Description: Function to display New Password options
 */

void DisplayNewPasswordCheck (void);

/*
 * Description: Function to display Enter Password options
 */

uint8 DisplayEnterPasswordCheck(void);

/*
 * Description : Function to control the actuation of the buzzer and DC motor
 */

void OpenDoorApplication (void);

/*
 * Description : Function to control the Change of password instruction
 */

void ChnagePasswordApplication (void);

/*
 * Description: Function that control the main menu display options
 */

void MainMenuCheck (void);


#endif /* MC1_FUNCTION_H_ */
