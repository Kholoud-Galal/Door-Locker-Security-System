/******************************************************************************
 *
 * File Name: ECU_Controller.h
 *
 * Description: Header file for the HIM_Controller in application layer
 *
 * Author: Kholoud Galal
 *
 *******************************************************************************/

#ifndef ECU_CONTROLLER_H_
#define ECU_CONTROLLER_H_

#include"std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define WRONG_STATE                    0x01
#define INITIAL_STATE                  0x02
#define MAXIMUM_NUMBER_OF_TRIES        0x03
#define RIGHT_STATE                    0x04
#define LENGTH_OF_PASSWORD             0x05
#define UPDATE_PASSWORD_STATE          0x06
#define OPEN_DOOR_STATE                0x07
#define READY                          0x08
#define NOT_READY                      0x09
#define DONE                           0x0A
#define SAVED                          0x0C
#define NOT_SAVED                      0x0D
#define ONE_MINUTE                     240
#define FIFTEEN_SECOND                 60
#define EEPROM_DEFUALT_VALUE           0XFF

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/

volatile uint8 tick=0;
uint8 FirstEnteredPassword[LENGTH_OF_PASSWORD];
uint8 SecondEnteredPassword[LENGTH_OF_PASSWORD];
uint8 password[LENGTH_OF_PASSWORD];
uint8 stored_password[LENGTH_OF_PASSWORD];
uint8 state=INITIAL_STATE;
uint8 check=WRONG_STATE;
uint8 tries=0;
uint8 PasswordState;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
* Description:
*  Function to check if there is a previous saved password in the External EEPROM
*/
uint8 CheckIfPasswordSaved(void);
/*
 * Description:
 *  Function to receive the first entered password from the HIM_Controller
 */
void ReceiveFirstPassword(void);
/*
 * Description:
 *  Function to receive the second entered password from the HIM_Controller
 */
void ReceiveSecondPassword (void);
/*
 * Description:
 *  Function to check matching between the two Received passwords from the HIM_Controller
 */
uint8 CheckReceivedPassword(void);
/*
 * Description:
 *  Function to store confirmed password in the External EEPROM
 */
void StoreReceivedPassword (void);
/*
 * Description:
 *  Function to receive the required option(update password or open door) from the HIM_Controller
 */
uint8 RecieveState (void);
/*
 * Description:
 *  Function to receive the entered password from the HIM_Controller
 */
void ReceivePassword(void);
/*
 * Description:
 * Function to check matching between the stored password in the External EEPROM and Received password from the HIM_Controller
 */
uint8 CheckMatching (void);
/*
 * Description:
 *  Function to read the saved password in the External EEPROM
 */
void ReadSavedPassword(void);
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
 *  Function to open the system door for 15 second
 */
void DoorOpen (void);
/*
 * Description:
 *  Function to close the system door for 15 second
 */
void DoorClose (void);
/*
* Description:
*  Function to activate the alarm system for 1 minute.
*/
void ActivateAlarmSysyem(void);

#endif /* ECU_CONTROLLER_H_ */
