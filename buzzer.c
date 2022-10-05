/*
 * buzzer.c
 *
 *  Created on: Jun 18, 2022
 *      Author: engim
 */

#include"buzzer.h"
#include"std_types.h"
#include"gpio.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the buzzer driver.
 */
void buzzer_init(void)
{
	GPIO_setupPinDirection(PORTB_ID,PIN0_ID,PIN_OUTPUT);
	/*Initial value*/
	GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
}
/*
 * Description :
 * Function responsible for activate the buzzer.
 */
void buzzer_on(void)
{
	GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_HIGH);
}
/*
 * Description :
 * Function responsible for de-activate the buzzer.
 */
void buzzer_off(void)
{
	GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
}
