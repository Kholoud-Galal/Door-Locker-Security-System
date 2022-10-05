/******************************************************************************
 *
 * File Name: HIM_Controller.h
 *
 * Description: Header file for the HIM_Controller in application layer
 *
 * Author: Kholoud Galal
 *
 *******************************************************************************/

#ifndef HIM_CONTROLLER_H_
#define HIM_CONTROLLER_H_

#include"std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define WRONG_STATE                      0x01
#define INITIAL_STATE                    0x02
#define MAXIMUM_NUMBER_OF_TRIES          0x03
#define RIGHT_STATE                      0x04
#define LENGTH_OF_PASSWORD               0x05
#define UPDATE_PASSWORD_STATE            0x06
#define OPEN_DOOR_STATE                  0x07
#define READY                            0x08
#define NOT_READY                        0x09
#define DONE                             0x0A
#define SAVED                            0x0C
#define NOT_SAVED                        0x0D
#define EEPROM_DEFUALT_VALUE             0XFF
#define ONE_MINUTE                       240
#define FIFTEEN_SECOND                   60

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
volatile uint8 tick=0;
uint8 FirstEnteredPassword[LENGTH_OF_PASSWORD];
uint8 SecondEnteredPassword[LENGTH_OF_PASSWORD];
uint8 password[LENGTH_OF_PASSWORD];
uint8 state = INITIAL_STATE;
uint8 check= WRONG_STATE;
uint8 tries = 0;
uint8 PasswordState;
uint8 Alarm=0;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description:
 *  Function to display the main two option of the system
 */
void MainOptionOfTheSystem (void);
/*
 * Description:
 *  Function to select the required system option using keypad
 */
uint8 SystemChooseOption (void);
/*
 * Description:
 *  Function to check if the system password already setted in the External EEPROM
 *  Connected with the ECU_Controller or this is the first activation.
 */
uint8 ifPassSavedORNOT(void);
/*
 * Description:
 *  Function to set the password of the system by asking the user to enter 5 numbers of password
 *  then send them to the ECU_Controller
 */
void SetPasswordOfTheSystem (void);
/*
 * Description:
 *  Function to confirm the entered password of the system by asking the user to re-enter the password again
 *  then send them to the ECU_Controller
 */
void ConfirmEnteredPassword (void);
/*
* Description:
*  Function to check matching of the two entered password by sending them to the ECU_Controlle and receive its replay
*/
uint8 ChechPasswordMatching (void);
/*
* Description:
*  Function to compare the entered password with the saved one in the external EEPROM that connected to the ECU_Controller
*/
uint8 EnterPassword (void);
/*
* Description:
*  Function to send the password to the ECU_Controller through UART.
*/
void SendPassword(uint8 *a_arr);
/*
* Description:
*  Function to send the Required Option to the ECU_Controller through UART.
*/
void SendState(uint8 status);
/*
* Description:
*  function that will be passed to the callBack function.
*/
void Timer0CounterProcessing(void);
/*
* Description:
*  Function to initialize timer and set its callBack function.
*/
void TimerStart(void);
/*
 * Description:
 *  Function to display Message of opening the door of the system for 15 seconds
 */
void OpenDoorOfTheSystem (void);
/*
 * Description:
 *  Function to display Message of opening the door of the system for 15 seconds
 */
void CloseDoorOfTheSystem (void);
/*
 * Description:
 *  Function to display Message of activation alarm system for 15 1 minute
 */
void AlarmSystemMessage(void);
/*
 * Description:
 *  Function to display Message to confirming that the password saved
 *  On the External EEPROM that connected to the ECU_Controller
 */
void ConfirmPasswordSave (void);

#endif /* HIM_CONTROLLER_H_ */
