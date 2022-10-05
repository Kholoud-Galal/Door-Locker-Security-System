/******************************************************************************
 *
 * Module: Timer Module
 *
 * File Name: timer.h
 *
 * Description: Header file for the ATmega16 Timer Module driver
 *
 * Author: Kholoud Galal
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      User Defined Data Type                                 *
 *******************************************************************************/
typedef enum
{
	TIMER0,TIMER1,TIMER2
}Timer_Number;
typedef enum
{
	A,B,NO_CHANNEL
}Timer_Channel;
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	OVERFLOW=0,COMPARE=2
	/* the compare =2 to fit the insertion equation of the TIMSK register */
}Timer_Mode;
typedef enum
{
	DISABLE,ENABLE
}Timer_Interrupt_Mode;

typedef struct
{
	Timer_Number timer;
	Timer_Channel channel_number;
	Timer_Clock clock;
	Timer_Mode mode;
	Timer_Interrupt_Mode interruptMode;
	uint16 initialValue ;
	uint16 compareValue ;
}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description:
 *  Function to initiate all Timers(mode,prescaller,initial value,compare value,interrupt).
 *  Choose Timer Interrupt Mode
 *  Choose Timer initial value
 *  Choose Timer_ID (Timer0, Timer1, Timer2)
 *  Choose Timer_Mode (OverFlow, Compare)
 *  Choose Timer compare match value if using CTC mode
 *  Choose Timer_Clock
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);
/*
 * Description:
 * Function to deinit the Timers
 */
void Timer_DeInit(uint8 timer_number);
/*
 * Description :
 * Function to clear the Timers Value to start count from ZERO.
 */
void Timer_clearTimerValue(uint8 timer_number);

/*
 * Description:
 * Function to set the CallBack function of timer0.
 */
void Timer0_setCallBack( void(*a_Timer0_ptr)(void));
/*
 * Description:
 * Function to set the CallBack function of timer1.
 */
void Timer1_setCallBack( void(*a_Timer1_ptr)(void));
/*
 * Description:
 * Function to set the CallBack function of timer2.
 */
void Timer2_setCallBack( void(*a_Timer2_ptr)(void));

#endif /* TIMER_H_ */
