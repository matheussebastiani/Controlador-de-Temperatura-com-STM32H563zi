/*
 * pwm.c
 *
 *  Created on: Nov 22, 2025
 *      Author: matheus, gustavo
 */

#include "pwm.h"
#include "tim.h"
#include <stdint.h>

/*
 * 	Essa função deverá:
 * 1. Inicializar o PWM indicado;
 * 2. Setar inicialmente o Duty Cicle para 0.
 * 3. Poderá ser utilizado para ambos os PWMs, desde que o canal seja passado corretamente
 */

void PWM_Init(TIM_HandleTypeDef *htim, uint32_t canal){

	__HAL_TIM_SET_COMPARE(htim, canal, PWM_DUTY_CICLE_DESLIGADO); // Seta o Duty Cicle para 0

	HAL_TIM_PWM_Start(htim, canal);
}

/*
 *	Essa função deverá:
 * 1. Receber um valor de Duty Cicle do padrão NEXTION
 * 2. Converter para o padrão de 3.3V = VMAX para o resistor
 * 3. Aplicar o valor no CCR do Timer
 * Pino de saída desse PWM: PC6 (D16)
 */

void PWM_Resistor_SetDutyCycle(float duty_percent_nextion){

	float ccr_float = 0.0f;
	float duty_percent_real = 0.0f; // Necessário ajustar para os valores possíveis
	uint32_t ccr = 0;
	uint32_t valor_arr = htim3.Instance->ARR;

	if(duty_percent_nextion > PWM_MAX)
		duty_percent_nextion = PWM_MAX;
	else if(duty_percent_nextion < PWM_MIN)
		duty_percent_nextion = PWM_MIN;

	duty_percent_real = (duty_percent_nextion * FATOR_CONVERSAO_DUTY_PWM_RESISTOR_NEXTION);

	ccr_float = (duty_percent_real / 100.0f) * (valor_arr + 1);
	ccr = (uint32_t)(ccr_float); // Truncando o ccr para uint32_t

	htim3.Instance->CCR1 = ccr; // Atualiza o valor do CCR1 para termos o Duty Cicle correto.

}

/*
 * PWM Normal. Faz a mesma coisa que a de cima só que sem ajustar para Duty Max = 27,5%
 * Pino de saída desse PWM: PD12 (D29)
 */

void PWM_SetDutyCycle(float duty_percent_nextion){
	float ccr_float = 0.0f;
	uint32_t ccr = 0;
	uint32_t valor_arr = htim4.Instance->ARR;

	if(duty_percent_nextion > PWM_MAX)
		duty_percent_nextion = PWM_MAX;
	else if(duty_percent_nextion < PWM_MIN)
		duty_percent_nextion = PWM_MIN;

	ccr_float = (duty_percent_nextion / 100.0f) * (valor_arr + 1);
	ccr = (uint32_t)(ccr_float); // Truncando o ccr para uint32_t

	htim4.Instance->CCR1 = ccr; // Atualiza o valor do CCR1 para termos o Duty Cicle correto.

}

/*
 * Essa função desabilitará o PWM de determinado htim e determinado canal
 */

void PWM_Stop(TIM_HandleTypeDef *htim, uint32_t canal){

	HAL_TIM_PWM_Stop(htim, canal);

}


