/*
 * lm35.h
 *
 *  Created on: Nov 22, 2025
 *      Author: matheus, gustavo
 *
 *  Neste arquivo contém: definição de constantes e funções para a leitura do LM35 e conversão de
 *  volts para °C
 */

#ifndef INC_LM35_H_
#define INC_LM35_H_

/* Includes */

#include "adc.h"

/* Includes */

#define RESOLUCAO_ADC_LM35 		  4095.0f
#define TENSAO_REFERENCIA_AD 	  3.3f
#define FATOR_CONVERSAO_LM35_VOLS 100.0f		// Como 1°C = 10mV
#define NUM_MEDICOES 			  10.0f

/* Variáveis Exportadas */

// Isso permitirá utilizar essa variável em outros arquivos, apenas incluindo o lm35.h

extern float temperaturaAtual;

/* Variáveis Exportadas */

/* Funções do LM35: */

float calculaTemperatura();

float ADCtoVolts(ADC_HandleTypeDef *adc_channel);

/* _Funções do LM35 */

#endif /* INC_LM35_H_ */
