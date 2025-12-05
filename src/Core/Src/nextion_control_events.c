/*
 * nextion_control_events.c
 *
 *  Created on: Dec 4, 2025
 *      Author: Matheus e Gustavo
 */
#include "nextion_control_events.h"
#include "nextion_events.h"
#include "nextion.h"
#include <stdio.h>

//nesse arquio esta toda a logica de funcionamento das funcoes delcaradas no arquivo HEADER, ira interpretar os evntos
//recebidos do nextion e ira decidir qual decisrao ira tomar

EstadoSistema_t EstadoAtual;
Controlador_Temp_Infos_t InfosControlador;

//abaixo estao definidos os HANDLERS ou os MANIPULADORES intenrnos para os eventos
static void nextion_handle_estado_padrao(Nextion_event_t *evento);
static void nextion_handle_estado_sp(Nextion_event_t *evento);
static void nextion_handle_estado_kp(Nextion_event_t *evento);
static void nextion_handle_estado_driver(Nextion_event_t *evento);
static void nextion_handle_estado_aquecedor(Nextion_event_t *evento);
static void nextion_handle_estado_fan(Nextion_event_t *evento);
static void nextion_handle_estado_page_manual(Nextion_event_t *evento);
static void nextion_handle_estado_page_automatico(Nextion_event_t *evento);


//inicializacao do estado da maquinda (estado inicial)
void Nextion_controle_eventos_init(void)
{
	EstadoAtual = STATE_PADRAO;

	/* Inicialização da estrutura principal de controle	*/

	InfosControlador.sp = VALOR_INICIAL_SP;
	InfosControlador.pv = VALOR_INICIAL_PV;
	InfosControlador.kp = VALOR_INICIAL_KP;

	InfosControlador.pag_atual = PAGINA_INICIAL;

	InfosControlador.driver_on = ESTADO_INICIAL_DRIVER;
	InfosControlador.fan_on = ESTADO_INICIAL_FAN;
	InfosControlador.heater_on = ESTADO_INICIAL_AQUECEDOR;

	InfosControlador.fan_dt = DT_PWM_INICIAL_FAN;
	InfosControlador.heater_dt = DT_PWM_INICIAL_AQUECEDOR;

	InfosControlador.modo_seguranca = MODO_SEGURANCA_INICIAL;

}

//a funcao abaixo de controle deve ser chamada no loop, eela tenta pegar um evneot da fila (event_dequeue)
//se nao existir retrorna
//se existir, manda para o manipulador correto dependendo do estado atual
void Nextion_controle_eventos_run(void)
{
	//tentando buscar evento na fila circular
	Nextion_event_t evento = event_dequeue(&FilaEventos);

	//se nao houver um evento
	if (evento.event == -1 && evento.value == -1){
		return;
	}

	if(EstadoAtual == STATE_PADRAO)
		nextion_handle_estado_padrao(&evento);

	//agora sim iremos passar o evneto para o handler correto
	switch (EstadoAtual)
	{

	case STATE_ESPERA_SP:
		nextion_handle_estado_sp(&evento);
		break;

	case STATE_ESPERA_KP:
		nextion_handle_estado_kp(&evento);
		break;

	case STATE_CONTROLE_DRIVER:
		nextion_handle_estado_driver(&evento);
		break;

	case STATE_CONTROLE_AQUECEDOR:
		nextion_handle_estado_aquecedor(&evento);
		break;

	case STATE_CONTROLE_FAN:
		nextion_handle_estado_fan(&evento);
		break;

	case STATE_PAGINA_MANUAL:
		nextion_handle_estado_page_manual(&evento);
		break;

	case STATE_PAGINA_AUTOMATICO:
		nextion_handle_estado_page_automatico(&evento);
		break;

	default:
		//se o estado for invalido, volta ao padrao
		EstadoAtual = STATE_PADRAO;
		break;

	}
}

//a seguir estao as implementacoes dos handlers para cada estado


//padrao
static void nextion_handle_estado_padrao(Nextion_event_t *evento){
	switch(evento->event)
	{
	case EVENT_CONFIRM_SP:
		EstadoAtual = STATE_ESPERA_SP;
		break;

	case EVENT_CONFIRM_KP:
		EstadoAtual = STATE_ESPERA_KP;
		break;

	case EVENT_TOGGLE_DRIVER:
		EstadoAtual = STATE_CONTROLE_DRIVER;
		break;

	case EVENT_TOGGLE_HEATER:
		EstadoAtual = STATE_CONTROLE_AQUECEDOR;
		break;

	case EVENT_TOGGLE_FAN:
		EstadoAtual = STATE_CONTROLE_FAN;
		break;

	case EVENT_PAGE_MANUAL:
		EstadoAtual = STATE_PAGINA_MANUAL;
		break;

	case EVENT_PAGE_AUTO:
		EstadoAtual = STATE_PAGINA_AUTOMATICO;
		break;

	default:
		break;
	}
}

//SP
static void nextion_handle_estado_sp(Nextion_event_t *evento){
	switch(evento->event)
	{
	case EVENT_CONFIRM_SP:
		InfosControlador.sp = evento->value;
		break;

	default:
		EstadoAtual = STATE_PADRAO;
	}
}

//KP
static void nextion_handle_estado_kp(Nextion_event_t *evento){
	switch(evento->event)
	{
	case EVENT_CONFIRM_KP:
		InfosControlador.kp = evento->value;
		break;

	default:
		EstadoAtual = STATE_PADRAO;
		break;
	}
}

//driver
static void nextion_handle_estado_driver(Nextion_event_t *evento){
	if (evento->event == EVENT_TOGGLE_DRIVER){

		if(InfosControlador.pag_atual == PAGINA_AUTOMATICO)
			InfosControlador.driver_on = evento->value; // Como sabemos o que será retornado do nextion, podemos fazer isso

		else if (InfosControlador.pag_atual == PAGINA_MANUAL) // No modo manual, o driver está sempre ligado
			InfosControlador.driver_on = LIGADO;
	}
	EstadoAtual = STATE_PADRAO;
}

//aquecedor
static void nextion_handle_estado_aquecedor(Nextion_event_t *evento){
    switch(evento->event)
    {
        case EVENT_TOGGLE_HEATER:
            	if(InfosControlador.pag_atual == PAGINA_MANUAL)
            		InfosControlador.heater_on = evento->value;
            	else if(InfosControlador.pag_atual == PAGINA_AUTOMATICO)
            		InfosControlador.heater_on = LIGADO;
            break;

        case EVENT_CONFIRM_HEATER_VALUE:            //CONSIDERAR QUE O VALOR PODE ESTAR SENDO ALTERADO MESMO COM O DRIVER DESLIGADO NA INTERFACE

        	if(InfosControlador.pag_atual == PAGINA_MANUAL) // Apenas necessita de confirmação na página manual. Na parte automática, não mexe
        		InfosControlador.heater_dt = evento->value;

        	break;

        default:
        	break;
    }
    EstadoAtual = STATE_PADRAO;
}

//fan
static void nextion_handle_estado_fan(Nextion_event_t *evento){
	switch(evento->event)
	{
	case EVENT_TOGGLE_FAN:

		if(InfosControlador.pag_atual == PAGINA_MANUAL){
			InfosControlador.fan_on = evento->value;

		} else if(InfosControlador.pag_atual == PAGINA_AUTOMATICO){
			InfosControlador.fan_on = LIGADO;
		}

		break;

	case EVENT_CONFIRM_FAN_VALUE:                    //CONSIDERAR QUE O VALOR PODE ESTAR SENDO ALTERADO MESMO COM O DRIVER DESLIGADO NA INTERFACE

		if(InfosControlador.pag_atual == PAGINA_MANUAL){ // Apenas mexe no duty cicle se for no modo manual
			InfosControlador.fan_dt = evento->value;
		}

		break;

	default:
		break;
	}
	EstadoAtual = STATE_PADRAO;
}

//pagina manual
/*
 * 	Em uma transição de página, no caso, estado de operação, deve-se garantir que a transição seja feita de maneira segura, para evitar
 * 	Possíveis mudanças bruscas na lógica de controle de uma hora para a outra
 *	Para isso, os PWMs serão setados em 0 inicialmente, e o driver estará ligado
 *
 * */

static void nextion_handle_estado_page_manual(Nextion_event_t *evento){

	switch(evento->event)
	{

	case EVENT_PAGE_MANUAL: 						// Necessário implementar a lógica de "transição segura"

		InfosControlador.modo_seguranca = LIGADO;

		InfosControlador.driver_on = LIGADO;
		InfosControlador.heater_on = DESLIGADO;
		InfosControlador.fan_on = DESLIGADO;

		InfosControlador.pag_atual = PAGINA_MANUAL;

		InfosControlador.fan_dt = DESLIGADO;
		InfosControlador.heater_dt = DESLIGADO;

		/* Variáveis não utilizadas deverão voltar ao estado inicial para serem recarregadas posteriormente	*/
		InfosControlador.kp = VALOR_INICIAL_KP;
		InfosControlador.sp = VALOR_INICIAL_SP;

		nextion_set_component_value(NEXTION_OBJNAME_PV, InfosControlador.pv); // Já envia ao Nextion a temperatura atual

		EstadoAtual = STATE_PADRAO;
		break;

	default:
		break;
	}
}

//pagina automatico
static void nextion_handle_estado_page_automatico(Nextion_event_t *evento){
	switch(evento->event)
	{

	case EVENT_PAGE_AUTO:

		InfosControlador.modo_seguranca = LIGADO;

		InfosControlador.driver_on = DESLIGADO;
		InfosControlador.heater_on = DESLIGADO;
		InfosControlador.fan_on = DESLIGADO;

		InfosControlador.pag_atual = PAGINA_AUTOMATICO;

		InfosControlador.fan_dt = DESLIGADO;
		InfosControlador.heater_dt = DESLIGADO;

		InfosControlador.kp = VALOR_INICIAL_KP;
		InfosControlador.sp = VALOR_INICIAL_SP;

		nextion_set_component_value(NEXTION_OBJNAME_PV, InfosControlador.pv); // Já envia ao Nextion a temperatura atual


		EstadoAtual = STATE_PADRAO;
		break;

	default:
		break;
	}
}
