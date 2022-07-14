/*
 ===============================================================================
 Name        : MC2.c
 Data        : Jun 29, 2022
 Author      : The Bayoumi
 Description :
 ===============================================================================
 */

#include "MC2_Function.h"

int main (void)
{

	uint8 Create_Flag=0;
	uint8 New_Flag=0;

	Buzzer_Init();
	DcMotor_init();
	TWI_Config Config_eepromm = {0x01,0x02};
	TWI_init(&Config_eepromm);
	UART_ConfigType UART_config = { 9600 , Eight_BIT , Disabled ,OneBIT };
	UART_init(&UART_config);

	while(1)
	{
		if (Create_Flag==0)
		{
			ReceiveNewPassword();
			Create_Flag =1 ;
		}
		if (New_Flag==0)
		{
			MenuOperationControl();
		}
	}
}

