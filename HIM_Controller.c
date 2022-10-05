/*
 ================================================================================================
 Name        : HMI_Controller.c
 Author      : Kholoud Galal
 Description : main function of door locker security system in the appliction layer
 Date        : 10/6/2022
 ================================================================================================
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include"lcd.h"
#include"keypad.h"
#include"timer.h"
#include"uart.h"

#include"HIM_Controller.h"

int main(void)
{
	/* Enable interrupts by setting I-bit */
	SREG  |= (1<<7);

	/* initialize LCD */
	LCD_init();

	/* initialize UART */
	usart_ConfigType UART_Config ={ASYNCHRONOUS,Disabled,One_Bit,Eight_Bits,9600};
	UART_init(&UART_Config);

	/* Start Message */
	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,2,"DOOR LOCKER");
	LCD_displayStringRowColumn(1,0,"SECURITY SYSTEM");
	//_delay_ms (4000);

	/*
	 * check if it is the first activation of the system or not
	 * by checking if there is any saved password in the external EEPROM.
	 */
	PasswordState=ifPassSavedORNOT();

	for(;;)
	{
		/*
		 *the user must set his security password in order to Use it later
		 */
		if(PasswordState==NOT_SAVED)
		{

			for(;;)
			{
				if(check==WRONG_STATE)
				{
					/*Enter the password in order to use it later*/
					SetPasswordOfTheSystem ();
					/*confirm entered password by re-enter it again*/
					ConfirmEnteredPassword ();
					/*check passwords matching*/
					check = ChechPasswordMatching ();
					/*if the state changed to right state then the loop will break otherwise back to set the password again*/
				}
				else if (check==RIGHT_STATE)
				{
					/*Display Message on LCD to confirm save*/
					ConfirmPasswordSave ();
					PasswordState=SAVED;
					break;
				}

			}
		}
		else if(PasswordState==SAVED)
		{
			/*waiting for the user to choose one of the two option available*/
			MainOptionOfTheSystem ();
			state = SystemChooseOption ();

			/*If the user want to update the system password*/
			if (state == UPDATE_PASSWORD_STATE)
			{
				/*Send The Required State To The ECU_Controller*/
				SendState(UPDATE_PASSWORD_STATE);
				/*Until Prove the opposite*/
				LCD_clearScreen ();
				LCD_displayStringRowColumn(0,0,"Enter Old Pass");
				LCD_displayStringRowColumn(1,0,"To Update It");
				_delay_ms (500);
				check= WRONG_STATE;
				/*check password first, the user have only three tries after that the alarm system will be activate*/
				while (check == WRONG_STATE && tries != MAXIMUM_NUMBER_OF_TRIES)
				{
					/*
					 * stuck the system here and increment the number of tries with every try
					 * the loop will break in two cases, first case when the state change to right state
					 * in this case the tries will be less than three
					 * the second case when the tries reach its maximum value then the alarm system will be activate
					 */
					check = EnterPassword ();

					if (check == WRONG_STATE)
					{
						LCD_clearScreen ();
						LCD_displayStringRowColumn(0,0,"WRONG PASSWORD");
						//_delay_ms (4000);
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
							/*Enter the password in order to use it later*/
							SetPasswordOfTheSystem ();
							/*confirm entered password by re-enter it again*/
							ConfirmEnteredPassword ();
							/*check passwords matching*/
							check = ChechPasswordMatching ();
						}
						else if (check == RIGHT_STATE)
						{
							/*Display Message on LCD to confirm save*/
							ConfirmPasswordSave ();
							break;
						}
					}
				}
				/*Second case*/
				else if (tries == MAXIMUM_NUMBER_OF_TRIES)
				{
					/*Infinite loop, system will stuck here for 1 minute and display warning message*/
					for(;;)
					{
						AlarmSystemMessage();
						if(Alarm==1)
						{
							tries=0;
							break;
						}
					}
				}

			}
			/*If the user want to open the door of the system*/
			else if (state == OPEN_DOOR_STATE)
			{
				/*Send The Required State To The ECU_Controller*/
				SendState(OPEN_DOOR_STATE);

				LCD_clearScreen ();
				LCD_displayStringRowColumn(0,0,"Enter Password");
				LCD_displayStringRowColumn(1,0,"To Open The Door");
				_delay_ms (500);

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
					check = EnterPassword ();

					if (check == WRONG_STATE)
					{
						LCD_clearScreen ();
						LCD_displayStringRowColumn(0,0,"WRONG PASSWORD");
						//_delay_ms (4000);
						tries++;
					}
				}
				/*First case*/
				if (tries != MAXIMUM_NUMBER_OF_TRIES)
				{
					tries = 0;
					OpenDoorOfTheSystem ();
					CloseDoorOfTheSystem ();
				}
				/*Second case*/
				else if (tries == MAXIMUM_NUMBER_OF_TRIES)
				{
					/*Infinite loop, system will stuck here for 1 minute and display warning message*/
					for(;;)
					{
						AlarmSystemMessage();
						if(Alarm==1)
						{
							tries=0;
							break;
						}
					}
				}
			}
			state = INITIAL_STATE;
			check = WRONG_STATE;
            Alarm=0;
			LCD_clearScreen ();
			LCD_displayStringRowColumn(0,0,"***Locking...***");
			_delay_ms(2000);
		}
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description:
 *  Function to display the main two option of the system
 */
void MainOptionOfTheSystem (void)
{
	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,0,"+: Change PW");
	LCD_displayStringRowColumn(1,0,"-: Open Door");
}
/*
 * Description:
 *  Function to select the required system option using keypad
 */
uint8 SystemChooseOption (void)
{
	uint8 pressed_key;
	pressed_key = KEYPAD_getPressedKey();
	_delay_ms (350);
	if ((pressed_key == '+') || (pressed_key == '-'))
	{
		if (pressed_key == '+')
		{
			return UPDATE_PASSWORD_STATE;
		}
		else if (pressed_key == '-')
		{
			return OPEN_DOOR_STATE;
		}
	}
	return -1;
}
/*
 * Description:
 *  Function to check if the system password already setted in the External EEPROM
 *  Connected with the ECU_Controller or this is the first activation.
 */
uint8 ifPassSavedORNOT(void)
{
	uint8 ReceivedCase;
	uint8 save;

	UART_sendByte (READY);

	ReceivedCase=(UART_recieveByte());

	/* wait for the ECU_Controller to make sure it's received and done */
	while(UART_recieveByte() != DONE){};

	switch (ReceivedCase)
	{
	case SAVED:
	{
		LCD_clearScreen ();
		LCD_displayStringRowColumn(0,0,"**Welcome Back**");
		save=SAVED;
		break;
	}
	case NOT_SAVED:
	{
		LCD_clearScreen ();
		LCD_displayStringRowColumn(0,2,"Set Password");
		LCD_displayStringRowColumn(1,0,"Of System First");
		save=NOT_SAVED;
		break;
	}

	}
	return save;
}
/*
 * Description:
 *  Function to set the password of the system by asking the user to enter 5 numbers of password
 *  then send them to the ECU_Controller
 */
void SetPasswordOfTheSystem (void) //first time use of the system
{
	uint8 i;

	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,0,"Set Password:");
	LCD_moveCursor(1,0);
	/* Store the first entered password in a global array to send it to the ECU_Controller*/
	for (i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		/*Get the pressed key on the KeyPad */
		FirstEnteredPassword[i]= KEYPAD_getPressedKey();
		/*delay for switch debouncing */
		_delay_ms (350);
		/*display it on the lcd */
		LCD_intgerToString(FirstEnteredPassword[i]);
		_delay_ms (100);
		LCD_moveCursor(1,i);
		LCD_displayCharacter ('*');
	}
	/* Send the first entered password to the ECU_Controller*/
	SendPassword(FirstEnteredPassword);
	LCD_clearScreen ();
}
/*
 * Description:
 *  Function to confirm the entered password of the system by asking the user to re-enter the password again
 *  then send them to the ECU_Controller
 */
void ConfirmEnteredPassword (void)
{
	uint8 i;

	LCD_displayStringRowColumn(0,0,"Re-Enter Please:");
	LCD_moveCursor(1,0);

	/* Store the first entered password in a global array to send it to the ECU_Controller*/
	for (i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		/*Get the pressed key on the KeyPad */
		SecondEnteredPassword[i]= KEYPAD_getPressedKey();
		/*delay for switch debouncing */
		_delay_ms (350);
		/*display it on the lcd */
		LCD_intgerToString(SecondEnteredPassword[i]);
		_delay_ms (100);
		LCD_moveCursor(1,i);
		LCD_displayCharacter ('*');
	}
	/* Send the second entered password to the ECU_Controller*/
	SendPassword(SecondEnteredPassword);
	LCD_clearScreen ();
}
/*
 * Description:
 *  Function to check matching of the two entered password sending them to the ECU_Controlle and receive its replay
 */
uint8 ChechPasswordMatching (void)
{
	UART_sendByte (READY);

	if (((UART_recieveByte ()) == RIGHT_STATE))
	{
		check=RIGHT_STATE;
		LCD_clearScreen ();
		LCD_displayStringRowColumn(0,0,"Confirmed Pass..");
		_delay_ms (500);
		LCD_clearScreen ();
		return RIGHT_STATE;
	}
	else if (((UART_recieveByte ()) == WRONG_STATE))
	{
		check=WRONG_STATE;
		LCD_clearScreen ();
		LCD_displayStringRowColumn(0,0,"Password Doesn't");
		LCD_displayStringRowColumn(1,2,"Match...");
		_delay_ms (500);
		LCD_clearScreen ();
		return WRONG_STATE;
	}

	else
	{
		return -1;
	}

}
/*
 * Description:
 *  Function to compare the entered password with the saved one in the External EEPROM that connected to the ECU_Controller
 */
uint8 EnterPassword (void) //every cycle
{
	uint8 i;
	uint8 ReceivedValue;

	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,0,"Enter Password:");
	LCD_moveCursor (1,0);

	/* Store the entered password in a global array to send it to the ECU_Controller*/
	for (i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		/*Get the pressed key on the KeyPad */
		password[i] = KEYPAD_getPressedKey();
		/*delay for switch debouncing */
		_delay_ms (350);
		LCD_displayCharacter ('*');
	}

	LCD_clearScreen ();
	/* Send the entered password to the ECU_Controller*/
	SendPassword(password);

	UART_sendByte (READY);
	ReceivedValue=UART_recieveByte ();
	return ReceivedValue;
}
/*
 * Description:
 *  Function to send the password to the ECU_Controller through UART.
 */
void SendPassword(uint8 *a_arr)
{
	int i;
	/* Sending UART ACK to the other ECU to make sure it's ready to receive */
	UART_sendByte (READY);
	for (i=0;i<LENGTH_OF_PASSWORD;i++)
	{
		/* Sending the password to the ECU_Controller */
		UART_sendByte (a_arr[i]);
	}
	/* wait for the ECU_Controller to make sure it's received and done */
	while(UART_recieveByte() != DONE){};

}
/*
* Description:
*  Function to send the Required Option to the ECU_Controller through UART.
*/
void SendState(uint8 status)
{
	/* Sending UART ACK to the other ECU to make sure it's ready to receive */
	UART_sendByte (READY);

	/* Sending the required state to the ECU_Controller */
	UART_sendByte (status);

	/* wait for the ECU_Controller to make sure it's received and done */
	while(UART_recieveByte() != DONE){};
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
 *  Function to display Message of opening the door of the system for 15 seconds
 */
void OpenDoorOfTheSystem (void)
{
	TimerStart();
	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,0,"Door is");
	LCD_displayStringRowColumn(1,5,"Opening...");
	while(tick!=FIFTEEN_SECOND);
	tick=0;
	Timer_DeInit(TIMER0);
	_delay_ms(2000);
}
/*
 * Description:
 *  Function to display Message of closing the door of the system for 15 seconds
 */
void CloseDoorOfTheSystem(void)
{
	TimerStart();
	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,0,"Door is");
	LCD_displayStringRowColumn(1,5,"Closing...");
	while(tick!=FIFTEEN_SECOND);
	tick=0;
	Timer_DeInit(TIMER0);
}
/*
 * Description:
 *  Function to display Message of activation alarm system for 15 1 minute
 */
void AlarmSystemMessage(void)
{
	TimerStart();
	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,0,"****WARNING****");
	while(tick!=ONE_MINUTE);
	tick=0;
	Timer_DeInit(TIMER0);
	Alarm=1;
}
/*
 * Description:
 *  Function to display Message to confirming that the password saved
 *  On the External EEPROM that connected to the ECU_Controller
 */
void ConfirmPasswordSave (void)
{
	LCD_clearScreen ();
	LCD_displayStringRowColumn(0,0,"*PASSWORD SAVED*");
	_delay_ms (500);
	LCD_clearScreen ();
}
