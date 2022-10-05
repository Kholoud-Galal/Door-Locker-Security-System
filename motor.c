/******************************************************************************
 *
 * Module: DC_Motor
 *
 * File Name: motor.c
 *
 * Description: Source file for the ATmega16 dc motor driver
 *
 * Author: Kholoud Galal
 *
 *******************************************************************************/
#include"motor.h"
#include "gpio.h" /* to set pins direction */

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the dc motor driver.
 */

void DcMotor_Init(void)
{
	/*
	 * Set the direction of two pin connected to H-bridge of Dc motor
	 * Give the two Pins initial value =0 (motor off)
	 */
	GPIO_setupPinDirection(MOTOR_PORT, MOTOR_PIN1, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT, MOTOR_PIN2, PIN_OUTPUT);
	/*initial value*/
	GPIO_writePin(MOTOR_PORT, MOTOR_PIN1, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT, MOTOR_PIN2, LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for rotate from the fan motor(clockwise,anti-clickwise,off)
 * with specific speed selected using PWM.
 */

void DcMotor_Rotate(DcMotor_State state)
{
	/*
	 * check motor rotate state first then set the H_bridge values
	 * to rotate according to the required state
	 * then set the speed according to the required value using timer0_pwm mode
	 */

	if(state==Motor_CW)
	{
		GPIO_writePin(MOTOR_PORT, MOTOR_PIN1, LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT, MOTOR_PIN2, LOGIC_LOW);
	}
	else if(state==Motor_ACW)
	{
		GPIO_writePin(MOTOR_PORT, MOTOR_PIN1, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT, MOTOR_PIN2, LOGIC_HIGH);

	}
	else if(state==Motor_OFF)
	{
		GPIO_writePin(MOTOR_PORT, MOTOR_PIN1, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT, MOTOR_PIN2, LOGIC_LOW);
	}

}

