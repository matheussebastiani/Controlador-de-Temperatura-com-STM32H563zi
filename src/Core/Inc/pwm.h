/*
 * pwm.h
 *
 *  Created on: Nov 22, 2025
 *      Author: matheus, gustavo
 *  Biblioteca onde ficarão as funções relacionadas ao controle dos PWMs utilizados
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

/* Includes */
#include <stdint.h>
#include "tim.h"

/* Definições */

#define PWM_RESISTOR_ARR 	 						2500-1
#define PWM_RESISTOR_PSC 	 						5-1
#define PWM_RESISTOR_CCR_MAX 						688

#define FATOR_CONVERSAO_DUTY_PWM_RESISTOR_NEXTION  0.275f
/*
 *	Como o Duty Cicle real que vamos ter que aplicar efetivamente no PWM é:
 *
 *	VALOR_DUTY_NEXTION * 27.5 / 100, resumimos isso em um "fator"
 *
 */


#define PWM_MAX 100.0f
#define PWM_MIN 0.0f
#define PWM_DUTY_CICLE_DESLIGADO 0

/* Definições */


/* Funções */

void PWM_Init(TIM_HandleTypeDef *htim, uint32_t canal);
void PWM_Resistor_SetDutyCycle(float duty_percent_nextion);
void PWM_SetDutyCycle(float duty_percent_nextion);
void PWM_Stop(TIM_HandleTypeDef *htim, uint32_t canal);


#endif /* INC_PWM_H_ */
