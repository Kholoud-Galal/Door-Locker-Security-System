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

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	ASYNCHRONOUS,SYNCHRONOUS
}usart_mode;

typedef enum
{
	Disabled,Reserved,Even_Parity,Odd_Parity
}usart_ParityMode;

typedef enum
{
	One_Bit,Two_Bits
}usart_stopBit;

typedef enum
{
	Five_Bit,Six_Bits,Seven_Bits,Eight_Bits,Reserved1,Resweved2,Reserved3,Nign_Bits
}usart_dataBit;

typedef struct
{
	usart_mode mode;
	usart_ParityMode parity;
	usart_stopBit stop_bit;
	usart_dataBit data;
	uint32 baud_rate;

}usart_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(usart_ConfigType * Config_Ptr);

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
