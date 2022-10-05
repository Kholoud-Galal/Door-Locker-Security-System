/******************************************************************************
 *
 * Module: DC_Motor
 *
 * File Name: motor.h
 *
 * Description: header file for the ATmega16 dc motor driver
 *
 * Author: Kholoud Galal
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h" /* to use them */

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define MOTOR_PORT       PORTB_ID
#define MOTOR_PIN1       PIN1_ID
#define MOTOR_PIN2       PIN2_ID

/*******************************************************************************
 *                      User Defined Data Type                                 *
 *******************************************************************************/

typedef enum
{
	Motor_CW,Motor_ACW,Motor_OFF
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the dc motor driver.
 */

void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for rotate from the fan motor(clockwise,anti-clickwise,off)
 * with specific speed selected using PWM.
 */

void DcMotor_Rotate(DcMotor_State state);


#endif /* MOTOR_H_ */
