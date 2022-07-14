 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "common_macros.h"

#define F_CPU 8000000UL
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

typedef enum {
	Five_BIT, Six_BIT, Seven_BIT, Eight_BIT, Nine_BIT = 7
}UART_DataBit;

typedef enum
{
	Disabled ,Reserved , Even, Odd
}UART_Parity;

typedef enum
{
	OneBIT,TwoBIT
}UART_NoStopBITS;

typedef struct {
	uint32 Baud_Rate;
	UART_DataBit Data_Bit;
	UART_Parity Parity;
	UART_NoStopBITS Stop_BIT;
}UART_ConfigType;

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
