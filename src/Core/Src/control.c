/*
 * control.c
 *
 *  Created on: Dec 5, 2025
 *      Author: matheus, gustavo
 */

#include "lm35.h"
#include "adc.h"
#include "nextion_control_events.h"
#include "nextion_interface.h"

void Controle_Temperatura(){



}


void Atualiza_Medidas(){

	temperaturaAtual = 0.0f; /* Global */

	calculaTemperatura(); /* temperaturaAtual é usada internamente aqui */

	InfosControlador.pv = (int32_t) temperaturaAtual;		// Vamos ter que truncar aqui

	return;

}

/* Lógica para o Controle das saídas no Modo Manual. As saídas não serão acionadas ainda */

void Controle_Modo_Manual(){

	if(!InfosControlador.heater_on){
		InfosControlador.heater_dt = DESLIGADO;
	}

	if(!InfosControlador.fan_on){
		InfosControlador.fan_dt = DESLIGADO;
	}

}


/* Lógica para o Controle das saídas no Modo Manual	*/

/*	Aqui, deverá ser feito
 * 1. Calculado o valor do erro.
 * 	 erro = SP - PV
 * 	 No caso, deve ser checado se o SP é maior que o PV.
 * 	 Caso o SP > PV -> Ligar o FAN;
 * 	 Caso o PV > SP -> Ligar o Heater;
 *
 * 2. Calcular o Duty Cicle
 * 	  No caso, será upwm Kp * |e|
 *
 */
void Controle_Modo_Automatico(){

	/* Vamos verificar quem é maior, o SP ou o PV */
	/* Fazer sempre ser positivo */

	if(!InfosControlador.driver_on){
		InfosControlador.fan_dt = DESLIGADO;
		InfosControlador.heater_dt = DESLIGADO;
		return;
	}

	if(InfosControlador.pv > InfosControlador.sp){
		InfosControlador.erro = (InfosControlador.pv - InfosControlador.sp);

		InfosControlador.fan_on = LIGADO;
		InfosControlador.heater_on = DESLIGADO;


	} else if(InfosControlador.sp > InfosControlador.pv){
		InfosControlador.erro = (InfosControlador.sp - InfosControlador.pv);

		InfosControlador.fan_on = DESLIGADO;
		InfosControlador.heater_on = LIGADO;
	}


	if(InfosControlador.heater_on){
		if(InfosControlador.kp > 0){
			InfosControlador.heater_dt = (InfosControlador.kp * InfosControlador.erro);
		}
	}

	if(InfosControlador.fan_on){
		if(InfosControlador.kp > 0){
			InfosControlador.fan_dt = (InfosControlador.kp * InfosControlador.erro);
		}
	}

}

/* Desliga todas as saídas */
void Controle_Modo_Segurança(){
	if(InfosControlador.modo_seguranca){
		InfosControlador.driver_on = DESLIGADO;
		InfosControlador.fan_on = DESLIGADO;
		InfosControlador.heater_on = DESLIGADO;

		InfosControlador.fan_dt = DESLIGADO;
		InfosControlador.heater_dt = DESLIGADO;
	}
}





