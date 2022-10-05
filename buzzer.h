/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: header file for the ATmega16 of buzzer driver
 *
 * Author: Kholoud Galal
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define BUZZER_PORT     PORTB_ID
#define BUZZER_PIN      PIN0_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the buzzer driver.
 */

void buzzer_init(void);
/*
 * Description :
 * Function responsible for activate the buzzer.
 */
void buzzer_on(void);
/*
 * Description :
 * Function responsible for de-activate the buzzer.
 */
void buzzer_off(void);

#endif /* BUZZER_H_ */
