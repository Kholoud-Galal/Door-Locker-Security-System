/******************************************************************************
 *
 * Module: Timer Module
 *
 * File Name: timer.c
 *
 * Description: Source file for the ATmega16 Timer Module.
 *
 * Author: Kholoud Galal
 *
 *******************************************************************************/

#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackTimer0Ptr)(void) = NULL_PTR;
static volatile void (*g_callBackTimer1Ptr)(void) = NULL_PTR;
static volatile void (*g_callBackTimer2Ptr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/**************************************************************************
 *                              Timer0                                    *
 * ************************************************************************/
ISR(TIMER0_OVF_vect)
{
	if(g_callBackTimer0Ptr != NULL_PTR)
	{
		/* Call the address of the function that given to the the CallBack function*/
		(*g_callBackTimer0Ptr)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackTimer0Ptr != NULL_PTR)
	{
		/* Call the address of the function that given to the the CallBack function*/
		(*g_callBackTimer0Ptr)();
	}
}

/**************************************************************************
 *                              Timer1                                    *
 * ************************************************************************/
ISR(TIMER1_OVF_vect)
{
	if(g_callBackTimer1Ptr != NULL_PTR)
	{
		/* Call the address of the function that given to the the CallBack function*/
		(*g_callBackTimer1Ptr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackTimer1Ptr != NULL_PTR)
	{
		/* Call the address of the function that given to the the CallBack function*/
		(*g_callBackTimer1Ptr)();
	}
}

ISR(TIMER1_COMPB_vect)
{
	if(g_callBackTimer1Ptr != NULL_PTR)
	{
		/* Call the address of the function that given to the the CallBack function*/
		(*g_callBackTimer1Ptr)();
	}
}

/**************************************************************************
 *                              Timer2                                    *
 * ************************************************************************/
ISR(TIMER2_OVF_vect)
{
	if(g_callBackTimer2Ptr != NULL_PTR)
	{
		/* Call the address of the function that given to the the CallBack function*/
		(*g_callBackTimer2Ptr)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackTimer2Ptr != NULL_PTR)
	{
		/* Call the address of the function that given to the the CallBack function*/
		(*g_callBackTimer2Ptr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
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
void Timer_init(const Timer_ConfigType *Config_Ptr) {

	/*******************************************************************************
	 *                                  TIMER0                                    *
	 *******************************************************************************/
	if((Config_Ptr->timer)==TIMER0)
	{
		/* Normal operation, OC0 disconnected */
		TCCR0 &= 0xCF ;

		/*because NON PWM MODE */
		SET_BIT(TCCR0,FOC0) ;

		/*insert the required mode in TCCR0 Register*/
		/*WGM00 is cleared because in both modes(overflow, compare) it equal to zero*/
		CLEAR_BIT(TCCR0,WGM00) ;

		/*insert the required mode by WGM01 shifted by three*/
		TCCR0 = (TCCR0 & 0xF7) | ((Config_Ptr->mode)<<3);

		/* insert the clock in the first three bits (CS10, CS11 and CS12) of TCCR0 Register
		 */
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);

		/* insert the intIial value in TCNT0 register */
		TCNT0=Config_Ptr->initialValue ;

		/* Insert the state of interrupt (enabled/disabled) */
		if((Config_Ptr->interruptMode)==ENABLE)
		{
			if((Config_Ptr->mode)==OVERFLOW)
			{
				TIMSK = (TIMSK & 0xFE) | (Config_Ptr->interruptMode);
			}
			else if((Config_Ptr->mode)==COMPARE)
			{
				TIMSK = (TIMSK & 0xFD) | ((Config_Ptr->interruptMode)<<1);
			}
		}
		else if((Config_Ptr->interruptMode)==DISABLE)
		{
			if((Config_Ptr->mode)==OVERFLOW)
			{
				TIMSK = (TIMSK & 0xFE) | (Config_Ptr->interruptMode);
			}
			else if((Config_Ptr->mode)==COMPARE)
			{
				TIMSK = (TIMSK & 0xFD) | ((Config_Ptr->interruptMode)<<1);
			}
		}

		/* insert the required compare value in OCR1A register */

		OCR0=Config_Ptr->compareValue ;

	}
	/*******************************************************************************
	 *                                  TIMER1                                    *
	 *******************************************************************************/

	else if((Config_Ptr->timer)==TIMER1)
	{

		/*******************************************************************************
		 *                               TIMER1_CHANNELA                               *
		 *******************************************************************************/

		if((Config_Ptr->channel_number)==A)
		{
			/* Normal port operation, OC1A disconnected */
			TCCR1A &= 0x30 ;

			/*NON PWM MODE */
			SET_BIT(TCCR1A,FOC1A) ;

			/*Clear Both WGM10 And WGM11 because in both CTC mode And OVERFLOW mode equal to zero*/
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);

			/*insert the required mode (overflow or compare)
			 * in WGM12 and WGM13 bits in TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xE7) | ((Config_Ptr->mode)<<3);

			/* insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->clock);

			/* insert the required intIial value in TCNT1 register */
			TCNT1=Config_Ptr->initialValue ;

			if((Config_Ptr->interruptMode)==ENABLE)
			{
				if((Config_Ptr->mode)==OVERFLOW)
				{
					TIMSK = (TIMSK & 0xFB) | ((Config_Ptr->interruptMode)<<2);
				}
				else if((Config_Ptr->mode)==COMPARE)
				{
					TIMSK = (TIMSK & 0xEF) | ((Config_Ptr->interruptMode)<<4);
				}
			}
			else if((Config_Ptr->interruptMode)==DISABLE)
			{
				if((Config_Ptr->mode)==OVERFLOW)
				{
					TIMSK = (TIMSK & 0xFB) | ((Config_Ptr->interruptMode)<<2);
				}
				else if((Config_Ptr->mode)==COMPARE)
				{
					TIMSK = (TIMSK & 0xEF) | ((Config_Ptr->interruptMode)<<4);
				}
			}

			/* insert the required compare value in OCR1A register */
			OCR1A=Config_Ptr->compareValue ;
		}
		/*******************************************************************************
		 *                               TIMER1_CHANNELB                               *
		 *******************************************************************************/


		if((Config_Ptr->channel_number)==B)
		{
			/* Normal port operation, OC1B disconnected */
			TCCR1A &= 0xC0 ;

			/*NON PWM MODE */
			SET_BIT(TCCR1A,FOC1B) ;

			/*Clear Both WGM10 And WGM11 because in both CTC mode And OVERFLOW mode equal to zero*/
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);

			/*insert the required mode (overflow or compare)
			 * in WGM12 and WGM13 bits in TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xE7) | ((Config_Ptr->mode)<<3);

			/* insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->clock);

			/* insert the required intIial value in TCNT1 register */
			TCNT1=Config_Ptr->initialValue ;

			if((Config_Ptr->interruptMode)==ENABLE)
			{
				if((Config_Ptr->mode)==OVERFLOW)
				{
					TIMSK = (TIMSK & 0xFB) | ((Config_Ptr->interruptMode)<<2);
				}
				else if((Config_Ptr->mode)==COMPARE)
				{
					TIMSK = (TIMSK & 0xF7) | ((Config_Ptr->interruptMode)<<4);
				}
			}
			else if((Config_Ptr->interruptMode)==DISABLE)
			{
				if((Config_Ptr->mode)==OVERFLOW)
				{
					TIMSK = (TIMSK & 0xFB) | ((Config_Ptr->interruptMode)<<2);
				}
				else if((Config_Ptr->mode)==COMPARE)
				{
					TIMSK = (TIMSK & 0xF7) | ((Config_Ptr->interruptMode)<<4);
				}
			}

			/* insert the required compare value in OCR1A register */

			OCR1B=Config_Ptr->compareValue ;
		}

	}

	else if((Config_Ptr->timer)==TIMER2)
	{
		/* Normal port operation, OC2 disconnected */
		TCCR2 &= 0xCF ;

		/*NON PWM MODE */
		SET_BIT(TCCR2,FOC2) ;

		/*insert the required mode (overflow or compare)
		 * in WGM20 and WGM21 bits in TCCR2 Register
	    */
		/*WGM00 is cleared because in both modes(overflow, compare) it equal to zero*/
		CLEAR_BIT(TCCR2,WGM20) ;

		TCCR2 = (TCCR2 & 0xF7) | ((Config_Ptr->mode)<<3);

		/* insert the required clock value in the first three bits (CS10, CS11 and CS12)
		 * of TCCR2 Register
		 */
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->clock);

		/* insert the required intIial value in TCNT2 register */
		TCNT2=Config_Ptr->initialValue ;

		if((Config_Ptr->interruptMode)==ENABLE)
		{
			if((Config_Ptr->mode)==OVERFLOW)
			{
				TIMSK = (TIMSK & 0xBF) | ((Config_Ptr->interruptMode)<<6);
			}
			else if((Config_Ptr->mode)==COMPARE)
			{
				TIMSK = (TIMSK & 0x7F) | ((Config_Ptr->interruptMode)<<7);
			}
		}
		else if((Config_Ptr->interruptMode)==DISABLE)
		{
			if((Config_Ptr->mode)==OVERFLOW)
			{
				TIMSK = (TIMSK & 0xBF) | ((Config_Ptr->interruptMode)<<6);
			}
			else if((Config_Ptr->mode)==COMPARE)
			{
				TIMSK = (TIMSK & 0x7F) | ((Config_Ptr->interruptMode)<<7);
			}
		}

		/* insert the required compare value in OCR1A register */

		OCR2=Config_Ptr->compareValue ;
	}

}

/*
 * Description :
 * Function to clear the Timers Value to start count from ZERO.
 */

void Timer_clearTimerValue(uint8 timer_number)
{

	if(timer_number==TIMER0)
	{
		TCNT0 = 0;
	}
	if(timer_number==TIMER1)
	{
		TCNT1 =0;
	}
	if(timer_number==TIMER2)
	{
		TCNT2 =0;
	}

}

/*
 * Description:
 * Function to deinit the Timers
 */
void Timer_DeInit(uint8 timer_number)
{
	/* Clear All Timers Registers */

	if(timer_number==TIMER0)
	{
		TCCR0=0;
		TCNT0=0;
		OCR0=0;
	}
	if(timer_number==TIMER1)
	{
		TCCR1A=0;
		TCCR1B=0;
		TCNT1=0;
		OCR1A=0 ;
		OCR1B=0 ;
	}
	if(timer_number==TIMER2)
	{
		TCCR2=0;
		TCNT2=0;
		OCR2=0;
	}

}
/*
 * Description:
 * Function to set the CallBack function of timer0.
 */
void Timer0_setCallBack(void(*a_Timer0_ptr)(void))
{
	/* Save the address of the Callback function in a global variable */
	g_callBackTimer0Ptr = a_Timer0_ptr;
}
/*
 * Description:
 * Function to set the CallBack function of timer1.
 */
void Timer1_setCallBack(void(*a_Timer1_ptr)(void))
{
	/* Save the address of the Callback function in a global variable */
	g_callBackTimer1Ptr = a_Timer1_ptr;
}
/*
 * Description:
 * Function to set the CallBack function of timer2.
 */
void Timer2_setCallBack(void(*a_Timer2_ptr)(void))
{
	/* Save the address of the Callback function in a global variable */
	g_callBackTimer2Ptr = a_Timer2_ptr;
}

