/*
 * lm35.c
 *
 *  Created on: Nov 22, 2025
 *      Author: matheus, gustavo
 */

#include "lm35.h"
#include "adc.h"

/* Variáveis Globais */

float temperaturaAtual = 0.0f;
volatile uint32_t valorLidoAD;

/* Variáveis Globais */

/* float calculaTemperatura():
 * 	  Essa função deverá:
 * 	1. Iniciar o ADC;
 * 	2. Fazer Polling do resultado;
 * 	3. Converter o valor para tensão (V)
 *	4. Efetuar o cálculo para converter para °C (PROCESS VALUE)
 *	5. Para evitar valores discrepantes, será feito uma média de 10 leituras
 */

float calculaTemperatura(){

	float tensaoLM35 = 0.0f;

	for(int i = 0; i < NUM_MEDICOES; i++){

		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, ADC_1_TIMEOUT);
		tensaoLM35 += ADCtoVolts(&hadc1);

		HAL_ADC_Stop(&hadc1);
	}

	tensaoLM35 = tensaoLM35 / NUM_MEDICOES;

	temperaturaAtual = tensaoLM35 * FATOR_CONVERSAO_LM35_VOLS;

	return temperaturaAtual;

}

/* float ADCtoVolts(ADC_HandleTypeDef *adc_channel):
 * 	  Helper da função float calculaTemperatura();
 * 	  Basicamente dará um GetValue no canal de AD e
 * 	  Retornará o valor em volts
 */

float ADCtoVolts(ADC_HandleTypeDef *adc_channel){

	float tensaoCalculada = 0;
	valorLidoAD = HAL_ADC_GetValue(adc_channel);

	tensaoCalculada = (valorLidoAD * TENSAO_REFERENCIA_AD) / RESOLUCAO_ADC_LM35;

	return tensaoCalculada;
}

