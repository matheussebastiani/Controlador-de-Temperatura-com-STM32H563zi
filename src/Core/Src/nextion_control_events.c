
#include "nextion_events.h"
#include "nextion.h"
#include "nextion_control_events.h"

Controlador_Temp_Infos_t InfosControlador;

void Nextion_controle_eventos_init(){

    InfosControlador.sp = VALOR_INICIAL_SP;
    InfosControlador.kp = VALOR_INICIAL_KP;
    InfosControlador.pv = VALOR_INICIAL_PV;
    InfosControlador.erro = 0;

    InfosControlador.driver_on  = DESLIGADO;
    InfosControlador.heater_on  = DESLIGADO;
    InfosControlador.fan_on     = DESLIGADO;

    InfosControlador.heater_dt  = 0;
    InfosControlador.fan_dt     = 0;

    InfosControlador.pag_atual = PAGINA_AUTOMATICO;
}

void Nextion_controle_eventos_run(){

	Nextion_event_t ev = event_dequeue(&FilaEventos);

	if(ev.event == EVENT_NONE){
		return;
	}

	switch(ev.event){

		case EVENT_CONFIRM_SP:
			InfosControlador.sp = ev.value;
			break;

		case EVENT_CONFIRM_KP:
			InfosControlador.kp = ev.value;
			break;

		case EVENT_TOGGLE_DRIVER:
			InfosControlador.driver_on = ev.value;
			break;

		case EVENT_CONFIRM_HEATER_VALUE:
			InfosControlador.heater_dt = ev.value;

			break;

		case EVENT_CONFIRM_FAN_VALUE:
			InfosControlador.fan_dt = ev.value;
			break;

		case EVENT_TOGGLE_FAN:
			InfosControlador.fan_on = ev.value;
			break;

		case EVENT_TOGGLE_HEATER:
			InfosControlador.heater_on = ev.value;
			break;

		case EVENT_PAGE_MANUAL:
			InfosControlador.pag_atual = PAGINA_MANUAL;
			InfosControlador.modo_seguranca = LIGADO;
			break;

		case EVENT_PAGE_AUTO:
			InfosControlador.pag_atual = PAGINA_AUTOMATICO;
			InfosControlador.modo_seguranca = LIGADO;
			InfosControlador.kp = VALOR_INICIAL_KP;
			InfosControlador.sp = VALOR_INICIAL_SP;
			break;

		default:
			break;
	}

}
