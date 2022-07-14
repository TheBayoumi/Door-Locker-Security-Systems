/*
 * MC1.c
 *
 *  Created on: Jun 22, 2022
 *      Author: The Bayoumi
 */


#include "MC1_Function.h"

int main(void)
{
	/*Initialize LCD*/
	LCD_init();

	uint8 Create_Flag=0;
	uint8 New_Flag=0;

	/*Configuration of UART:
	 * 1 : BIT rate = 9600
	 * 2 : No of data BIT = 8 BITS
	 * 3 : Parity : Disable
	 * 4 : No of stop BITS = 1 bit
	 */
	UART_ConfigType UART_config = { 9600 , Eight_BIT , Disabled ,OneBIT };
	UART_init(&UART_config);

	while (1)
	{
		if (Create_Flag==0)
		{
			DisplayNewPasswordCheck();
			Create_Flag =1 ;
		}

		if (New_Flag==0)
		{
			MainMenuCheck();
		}
	}
}
