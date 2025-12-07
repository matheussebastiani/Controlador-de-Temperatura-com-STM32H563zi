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
#include "control.h"
#include "pwm.h"
#include "nextion_events.h"
#include "nextion.h"

void Atualiza_PWM(){

	if(!InfosControlador.driver_on){
		PWM_Resistor_SetDutyCycle(DESLIGADO);
		PWM_SetDutyCycle(DESLIGADO);
		return;
	}
	//AQUECEDOR
	if(InfosControlador.heater_on){
		PWM_Resistor_SetDutyCycle((float) InfosControlador.heater_dt);
	} else {
		InfosControlador.heater_dt = DESLIGADO;
		PWM_Resistor_SetDutyCycle(DESLIGADO);
	}

	//FAN
	if(InfosControlador.fan_on){
		PWM_SetDutyCycle((float) InfosControlador.fan_dt);
	} else {
		InfosControlador.fan_on = DESLIGADO;
		PWM_SetDutyCycle(DESLIGADO);
	}
}

void Controle_Temperatura(){

	Atualiza_Medidas();

	if(InfosControlador.modo_seguranca){
		Controle_Modo_Segurança();
		return;
	}
	switch(InfosControlador.pag_atual){
		case PAGINA_MANUAL:
			Controle_Modo_Manual();
			break;

		case PAGINA_AUTOMATICO:
			Controle_Modo_Automatico();
			break;

		default:
			break;

	}


}

void Atualiza_Medidas(){
	if(InfosControlador.pv == VALOR_INICIAL_PV)
		temperaturaAtual = 0.0f; /* Global */

	calculaTemperatura(); /* temperaturaAtual é usada internamente aqui */

	InfosControlador.pv = (int32_t) temperaturaAtual;		// Vamos ter que truncar aqui

	if(InfosControlador.pag_atual == PAGINA_AUTOMATICO)
		nextion_set_component_value(NEXTION_OBJNAME_PV, (uint32_t)InfosControlador.pv);

	return;

}

/* Lógica para o Controle das saídas no Modo Manual. As saídas não serão acionadas ainda */

void Controle_Modo_Manual(){

	InfosControlador.driver_on = LIGADO; // No modo manual, o driver sempre estará ligado

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
		if(InfosControlador.kp >= 0){
			InfosControlador.heater_dt = (InfosControlador.kp * InfosControlador.erro);
		}
	}

	if(InfosControlador.fan_on){
		if(InfosControlador.kp >= 0){
			InfosControlador.fan_dt = (InfosControlador.kp * InfosControlador.erro);
		}
	}

	nextion_atualiza_fan(InfosControlador.fan_on);
	nextion_atualiza_aquecedor(InfosControlador.heater_on);

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
	InfosControlador.modo_seguranca = DESLIGADO;
}





