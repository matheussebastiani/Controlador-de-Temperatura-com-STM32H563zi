/*
 * nextion_interface.c
 *
 *  Created on: Dec 5, 2025
 *      Author: Matheus e Gustavo
 */

#include "nextion_interface.h"
#include "nextion.h"  //para o uso do nextion_set_component_value()
#include <stdio.h>
#include "nextion_control_events.h" // para o uso das variaveis

//atualizar o valor do PV
void nextion_atualiza_pv(int32_t pv){
	nextion_set_component_value("PV", (uint32_t) pv);

}

//atualizar o valor do SP
void nextion_atualiza_sp(int32_t sp){
	nextion_set_component_value("SP", (uint32_t) sp);
}

//atualizar o valor do kp
void nextion_atualiza_kp(int32_t kp){

	nextion_set_component_value("KP", (uint32_t) kp);
}

//atualizar o estado do driver(lembrando que "on" pode receber 0 ou 1 dependendo do estado)
void nextion_atualiza_driver(uint8_t driver_on){
	if (driver_on){
		nextion_set_component_text("DRIVER_STATE", "LIGADO");
	} else {
		nextion_set_component_text("DRIVER_STATE", "DESLIGADO");
	}
}

//atualizar o estado do fan
void nextion_atualiza_fan(uint8_t fan_on){
	if (fan_on){
		nextion_set_component_text("FAN_STATE", "LIGADO");
	} else {
		nextion_set_component_text("FAN_STATE", "DESLIGADO");
	}
}

//atualizar o estado do aquecedor
void nextion_atualiza_aquecedor(uint8_t heater_on){
	if(heater_on){
		nextion_set_component_text("HEAT_STATE", "LIGADO");
	} else {
		nextion_set_component_text("HEAT_STATE", "DESLIGADO");
	}
}

//atualizar o duty cycle do fan
void nextion_atualiza_fan_dt(int8_t fan_dt){
	nextion_set_component_value("F_VALUE", fan_dt);
}

//atualizar o duty cycle do aquecedor
void nextion_atualiza_aquecedor_dt(int8_t heater_dt){
	nextion_set_component_value("H_VALUE", heater_dt);

}

/*
 *
 *
 * como a questao do que deve estar inicializado nad paginas e com qual valor ja é tratado no case de cada pagina, resolvemos apenas deixar essas funcoes comentadas,
 * podemos mudar a logica depois para usa-las
 *

//trocar para a pagina manual
void nextion_goto_pagina_manual(void){
	nextion_set_page("MANUAL");
	nextion_set_component_value("H_VALUE", InfosControlador.heater_dt);
	nextion_set_component_value("F_VALUE", InfosControlador.fan_dt);
}

//trocar para a pagina autmoatico
void nextion_goto_pagina_automatico(void){
	nextion_set_page("AUTOMATICO");
	nextion_set_component_value("PV", InfosControlador.pv);
	nextion_set_component_value("SP", InfosControlador.sp);
	nextion_set_component_value("KP", InfosControlador.kp);
}
*/
