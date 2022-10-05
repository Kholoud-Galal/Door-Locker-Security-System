/*
 ================================================================================================
 Name        : CMU_Controller.c
 Author      : Kholoud Galal
 Description : main function of door locker security system in the appliction layer
 Date        : 10/6/2022
 ================================================================================================
 */

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"std_types.h"
#include"external_eeprom.h"
#include"uart.h"
#include"motor.h"
#include"buzzer.h"
#include"timer.h"
#include"ECU_Controller.h"

int main(void)
{
	/* Enable interrupts by setting I-bit */
	SREG  |= (1<<7);

	/* initialize EEPROM, dc-motor And Buzzer */
	EEPROM_init();
	DcMotor_Init();
	buzzer_init();

	/* initialize UART */
	usart_ConfigType UART_Config ={ASYNCHRONOUS,Disabled,One_Bit,Eight_Bits,9600};
	UART_init(&UART_Config);
	/*
	 * check if it is the first activation of the system or not
	 * by checking if there is any saved password in the external EEPROM.
	 */
	PasswordState=CheckIfPasswordSaved();

	for(;;)
	{

		if(PasswordState==NOT_SAVED)
		{
			for (;;)
			{
				if(check==WRONG_STATE)
				{
					/* Receive the first entered password from the HIM_Controller*/
					ReceiveFirstPassword();
					/* Receive the second entered password from the HIM_Controller*/
					ReceiveSecondPassword();
					/* check the two entered password from the HIM_Controller*/
					check = CheckReceivedPassword ();
				}
				else if(check==RIGHT_STATE)
				{
					PasswordState=SAVED;
					break;
				}
			}
		}
		else if(PasswordState==SAVED)
		{
			/*waiting for the user to choose one of the two option available*/
			state = RecieveState ();
			/*If the user want to update the system password*/
			if (state == UPDATE_PASSWORD_STATE)
			{
				/*Until Prove the opposite*/
				check=WRONG_STATE;
				/*check password first, the user have only three tries after that the alarm system will be activate*/
				while (check == WRONG_STATE && tries != MAXIMUM_NUMBER_OF_TRIES)
				{
					/*
					 * stuck the system here and increment the number of tries with every try
					 * the loop will break in two cases, first case when the state change to right state
					 * in this case the tries will be less than three
					 * the second case when the tries reach its maximum value then the alarm system will be activate
					 */
					check = CheckMatching ();

					if (check == WRONG_STATE)
					{
						/*in case the password dose not match then increment the number of tries*/
						tries++;
					}
				}

				/*First case*/
				if (tries != MAXIMUM_NUMBER_OF_TRIES)
				{
					/*reset the tries variable to start an new cycle next try*/
					tries = 0;
					/*Until Prove the opposite*/
					check = WRONG_STATE;
					for(;;)
					{
						if(check == WRONG_STATE)
						{
							/* Receive the first entered password from the HIM_Controller*/
							ReceiveFirstPassword();
							/* Receive the second entered password from the HIM_Controller*/
							ReceiveSecondPassword();
							/* check the two entered password from the HIM_Controller*/
							check = CheckReceivedPassword ();
						}
						else if(check == RIGHT_STATE)
						{
							break;
						}
					}
				}

				/*Second case*/
				else if (tries == MAXIMUM_NUMBER_OF_TRIES)
				{
					/*activate the alarm system for 1 min*/
					ActivateAlarmSysyem();
				}

			}
			/*If the user want to open the door of the system*/
			else if (state == OPEN_DOOR_STATE)
			{
				/*Until Prove the opposite*/
				check = WRONG_STATE;
				/*check password first, the user have only three tries after that the alarm system will be activate*/
				while (check == WRONG_STATE && tries != MAXIMUM_NUMBER_OF_TRIES)
				{
					/*
					 * stuck the system here and increment the number of tries with every try
					 * the loop will break in two cases, first case when the state change to right state
					 * in this case the tries will be less than three
					 * the second case when the tries reach its maximum value then the alarm system will be activate
					 */
					check = CheckMatching ();

					if (check == WRONG_STATE)
					{
						tries++;
					}
				}
				/*First case*/
				if (tries != MAXIMUM_NUMBER_OF_TRIES)
				{
					tries = 0;
					DoorOpen ();
					DoorClose();
				}
				/*Second case*/
				else if (tries == MAXIMUM_NUMBER_OF_TRIES)
				{
					/*activate the alarm system for 1 min*/
					ActivateAlarmSysyem();
					state=INITIAL_STATE;
				}
			}
		}

		/* Until prove the opposite and to start a new cycle */
		check = WRONG_STATE;
		state=INITIAL_STATE;

	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
* Description:
*  Function to check if there is a previous saved password in the External EEPROM
*/
uint8 CheckIfPasswordSaved(void)
{
	uint8 i;
	uint8 counter=0;
	static uint8 Enterance=0;
    /*in the first Enterance only*/
	if(Enterance==0)
	{
		Enterance++;
		/*wait for the HIM_Controller to be ready*/
		while ((UART_recieveByte ()) != READY);
		UART_sendByte(NOT_SAVED);
		/*to inform the HIM_Controller that receiving process is done*/
		UART_sendByte (DONE);
		return NOT_SAVED;
	}
	else
	{
		/*read if there is any saved password in the eeprom*/
		ReadSavedPassword();

		for(i=0 ; i < LENGTH_OF_PASSWORD; i++)
		{
			/*A factory-fresh will have 0xFF in every location*/
			if(stored_password[i] == EEPROM_DEFUALT_VALUE)
			{
				counter++;
			}

		}
		/*thats mean there is no saved password*/
		if (counter==LENGTH_OF_PASSWORD)
		{
			/*wait for the HIM_Controller to be ready*/
			while ((UART_recieveByte ()) != READY);
			/*send state to the HIM_Controller*/
			UART_sendByte(NOT_SAVED);
			/*to inform the HIM_Controller that receiving process is done*/
			UART_sendByte (DONE);
			return NOT_SAVED;
		}
		else
		{
			/*wait for the HIM_Controller to be ready*/
			while ((UART_recieveByte ()) != READY);
			/*send state to the HIM_Controller*/
			UART_sendByte(SAVED);
			/*to inform the HIM_Controller that receiving process is done*/
			UART_sendByte (DONE);
			return SAVED;
		}
	}

}
/*
 * Description:
 *  Function to receive the first entered password from the HIM_Controller
 */
void ReceiveFirstPassword(void)
{
	uint8 i;
	/*wait for the HIM_Controller to be ready*/
	while ((UART_recieveByte ()) != READY);
	/*Received the 5 integar first password in an array*/
	for(i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		FirstEnteredPassword[i]=UART_recieveByte();
	}
	/*to inform the HIM_Controller that receiving process is done*/
	UART_sendByte (DONE);

}
/*
 * Description:
 *  Function to receive the second entered password from the HIM_Controller
 */
void ReceiveSecondPassword (void)
{
	uint8 i;
	/*wait for the HIM_Controller to be ready*/
	while ((UART_recieveByte ()) != READY);
	/*Received the 5 integar second password in an array*/
	for(i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		SecondEnteredPassword[i]=UART_recieveByte();
	}
	/*to inform the HIM_Controller that receiving process is done*/
	UART_sendByte (DONE);
}
/*
 * Description:
 *  Function to check matching between the two passwords from the HIM_Controller
 */
uint8 CheckReceivedPassword (void)
{
	uint8 counter=0;
	uint8 i;
	/*compare between the two received passwords*/
	for(i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		/*if not matching increment the counter*/
		if(FirstEnteredPassword[i]!=SecondEnteredPassword[i])
		{
			counter++;
		}

	}
    /*if counter equal to zero thats mean the two passwords are identical*/
	if (counter == 0)
	{
		/*store the password in the eeprom*/
		StoreReceivedPassword ();
		/*wait for the HIM_Controller to be ready*/
		while ((UART_recieveByte ()) != READY);
		/*send the state to the HIM_Controller*/
		UART_sendByte (RIGHT_STATE);
		return RIGHT_STATE;
	}
	else

	{
		/*wait for the HIM_Controller to be ready*/
		while ((UART_recieveByte ()) != READY);
		/*send the state to the HIM_Controller*/
		UART_sendByte (WRONG_STATE);
		return WRONG_STATE;
	}
}
/*
 * Description:
 *  Function to store confirmed password using External EEPROM
 */
void StoreReceivedPassword (void)
{
	uint8 index;
	uint8 i;
	/* address of the eeprom */
	uint16 address=0x0302;
	/*storing the password in a global variable called stored_password*/
	for(i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		stored_password[i]=FirstEnteredPassword[i];
	}
     /*storing the password in the external eeprom*/
	for(index=0;index<LENGTH_OF_PASSWORD;index++)
	{
		/* Write in the external EEPROM */
		EEPROM_writeByte(address+index,stored_password[index]);
		/* delay time as the eeprom take 10ms to make a write*/
		_delay_ms(10) ;
	}
}
/*
 * Description:
 *  Function to receive the required option(update password or open door) from the HIM_Controller
 */
uint8 RecieveState (void)
{
	uint8 status;
	/*wait for the HIM_Controller to be ready*/
	while ((UART_recieveByte ()) != READY);
	/*receiving the required state from the HIM_Controller*/
	status=(UART_recieveByte ());
	/*to inform the HIM_Controller that receiving process is done*/
	UART_sendByte (DONE);

	return status;
}
/*
 * Description:
 *  Function to receive the entered password from the HIM_Controller
 */
void ReceivePassword(void)
{
	uint8 i;
	/*wait for the HIM_Controller to be ready*/
	while ((UART_recieveByte ()) != READY);
    /*Receive the entered password that comes from HIM_Controller*/
	for(i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		password[i]=UART_recieveByte();
	}
	/*to inform the HIM_Controller that receiving process is done*/
	UART_sendByte (DONE);

}
/*
 * Description:
 * Function to check matching between the stored password and user password from the HIM_Controller
 */
uint8 CheckMatching (void)
{
	uint8 i;
	uint8 counter=0;
    /*to check matching, first need to receive the entered password in the HIM_Controller then read the saved one un the eeprom*/
	ReceivePassword();
	ReadSavedPassword();

	/*compare between the stored password in eeprom and the received password*/
	for(i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		/*if not matching increment the counter*/
		if(password[i]!=stored_password[i])
		{
			counter++;
		}
	}
	/*if counter equal to zero thats mean the two passwords are identical*/
	if (counter== 0)
	{
		/*wait for the HIM_Controller to be ready*/
		while ((UART_recieveByte ()) != READY);
		/*send the state to the HIM_Controller*/
		UART_sendByte (RIGHT_STATE);

		return RIGHT_STATE;
	}
	else
	{
		/*wait for the HIM_Controller to be ready*/
		while ((UART_recieveByte ()) != READY);
		/*send the state to the HIM_Controller*/
		UART_sendByte (WRONG_STATE);
		return WRONG_STATE;
	}
}
/*
 * Description:
 *  Function to read the saved password in the External EEPROM
 */
void ReadSavedPassword(void)
{
	uint8 index;
	uint16 address=0x0302;
     /* read saved password in the eeprom */
	for(index=0 ; index<LENGTH_OF_PASSWORD ; index++)
	{
		EEPROM_readByte(address+index,&stored_password[index]);
		_delay_ms(10);
	}
}
/*
* Description:
*  function that will be passed to the callBack function.
*/
void Timer0CounterProcessing(void)
{
	tick++;
}
/*
* Description:
*  Function to initialize timer and set its callBack function.
*/
void TimerStart(void)
{
	Timer_ConfigType Timer_Config = {TIMER0,NO_CHANNEL,F_CPU_1024,OVERFLOW,ENABLE,6,0};
	Timer_init(&Timer_Config);
	Timer0_setCallBack(Timer0CounterProcessing);
}

/*
 * Description:
 *  Function to open the system door for 15 seconds
 */
void DoorOpen (void)
{
	/*initialize the timer*/
	TimerStart();
	/*rotate the motor in clockwise direction*/
	DcMotor_Rotate(Motor_CW);
	/*wait until the 15 seconds pass*/
	while(tick!=FIFTEEN_SECOND);
	/*reset tick value to start an new cycle*/
	tick=0;
    /*stop the motor*/
	DcMotor_Rotate(Motor_OFF);
	/*stop timer*/
	Timer_DeInit(TIMER0);
	/*holding timer between rotate clockwise and anti-clockwise*/
	_delay_ms(2000);
}

/*
 * Description:
 *  Function to close the system door for 15 seconds
 */
void DoorClose (void)
{
	/*initialize the timer*/
	TimerStart();
	/*rotate the motor in anti-clockwise direction*/
	DcMotor_Rotate(Motor_ACW);
	/*wait until the 15 seconds pass*/
	while(tick!=FIFTEEN_SECOND);
	/*reset tick value to start an new cycle*/
	tick=0;
    /*stop the motor*/
	DcMotor_Rotate(Motor_OFF);
	/*stop timer*/
	Timer_DeInit(TIMER0);
}
/*
* Description:
*  Function to activate the alarm system for 1 minute.
*/
void ActivateAlarmSysyem(void)
{
	/*initialize the timer*/
	TimerStart();
	/*activate the buzzer*/
	buzzer_on();
	/*wait until the 1 minute pass*/
	while(tick!=ONE_MINUTE);
	/*reset tick value to start an new cycle*/
	tick=0;
    /*stop the motor*/
	buzzer_off();
	/*stop timer*/
	Timer_DeInit(TIMER0);
}
