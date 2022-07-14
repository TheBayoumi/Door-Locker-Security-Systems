/*
 ===============================================================================
 Name        : MC2_Function.h
 Data        : Jun 29, 2022
 Author      : The Bayoumi
 Description : 
 ===============================================================================
 */
#ifndef MC2_FUNCTION_H_
#define MC2_FUNCTION_H_

#include "external_eeprom.h"
#include "DC_Motor.h"
#include "uart.h"
#include "twi.h"
#include "buzzer.h"
#include "timer.h"
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
 * Description:  Function to Store The Password into EEPROM
 */

void StorePassword (uint8 *StorePass_Ptr);

/*
 * Description: Function to Read The Password from EEPROM
 */

void ReadPassword (uint8 *StorePass_Ptr);

/*
 * Description: Function to Compare & Store Passwords
 * 1 : Compare between the passwords
 * 2 : If Password is Correct
 * 2 : Choose one of Two Password
 * 3 : Call StorePassword Function to Store The Chosen Password in The EEPROM
 */

uint8 PasswordCompareStore (uint8  PasswordSize , uint8 * Password1 , uint8 * Password2 );

/*
 * Description: Function to Receive New Password.
 * 1 : Waiting to Receive The First Password From HMI Microcontroller.
 * 2 : Waiting to Receive The Second Password From HMI Microcontroller.
 * 3 : Send The Two Passwords to PasswordCompareStore Function.
 * 4 : If Password Matched, Send to HMI Microcontroller value indicates that, Go to next step.
 * 5 : If Password Didn't Match, Send to HMI Microcontroller value indicates that.
 * 6 : Repeat.
 */

void ReceiveNewPassword (void);

/*
 * Description: Function for The Buzzer processing timer
 * 1 : Increment g_tick
 * 2 : if the g_tick incremented up to 1838(60 Seconds), Change The Flag value to 1
 */

void Buzzer_Processing_Time (void);

/*
 * Description: Function to Compare Between Two Passwords, Entered Password & Saved Password
 * 1 : Load The Stored Password from the EEPROM
 * 2 : Compare The Two Password
 * 3 : If the Password Match, Return value to indicate that the Password is Correct
 * 4 : If the Password didn't Match, Return value to indicate that the Password is Wrong
 * 5 : If the Password didn't Match more than 3 times, Return value to Start the Buzzer
 */

uint8 PasswordCompare (uint8  PasswordSize , uint8 * Password1 );

/*
 * Description: Function to Receive The Password
 * 1 : Waiting to Receive The Password From HMI Microcontroller
 * 2 : Send The Password to PasswordCompare Function
 * 3 : Returns a value that indicates weather the Password is Correct or Wrong or Alarm
 */

uint8 ReceiveComparePassword (void);

/*
 * Description: Function for The DC Motor processing timer
 * 1 : Increment g_ticks
 * 2 : if the g_ticks incremented up to 459(15 Seconds), Change The Flag value to 1
 * 3 : if the g_ticks incremented up to 643(18 Seconds), Change The Flag value to 2
 * 4 : if the g_ticks incremented up to 1102(33 Seconds), Change The Flag value to 0
 */

void DCMotor_Processing (void);

/*
 * Description: Function that control the Initialization of the Buzzer & DC Motor
 * 1 : Configure the Initialization of The Buzzer & DC Motor Timer0
 * 2 : Buzzer Timer0 (Normal Mode, Prescaler = 1024, InitialValue = 0, FinalValue = 255)
 * 3 : DC Motor Timer0 (Normal Mode, Prescaler = 1024, InitialValue = 0, FinalValue = 255)
 * 4 : Receive value indicates whether the password is Correct From ReceiveComparePassword's Function
 * 3 : If Password is Correct, Rotate The DC Motor Clockwise for 15 Seconds, Stop DC Motor for 3Seconds, Rotate The DC Motor Counter Clockwise for 15 Seconds
 * 4 : if Password isn't Correct more than 3 time start the Buzzer for 60 Seconds
 */

void OperationControl (void);

/*
 * Description: Function to change the password form the Main menu
 * 1: Receive The User Entered Password
 * 2: Check whether the Entered Password is Correct or not
 * 3: if Password is Correct, Create new Password
 * 4: if Password isn't Correct, Do nothing
 */

void ChangePasswordOperation (void);

/*
 * Description: Function to Check The Menu State
 * 1 : Waiting for the main menu state from HMI Microcontroller
 * 2 : if MenuDoorStatus was sent, Call OperationControl Function
 * 3 : if MenuChangingPassword was sent, Call ChangePasswordOperation Function
 */

void MenuOperationControl (void);

#endif /* MC2_FUNCTION_H_ */
