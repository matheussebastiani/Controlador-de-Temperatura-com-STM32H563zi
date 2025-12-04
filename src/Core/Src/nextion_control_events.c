/*
 * nextion_control_events.c
 *
 *  Created on: Dec 4, 2025
 *      Author: Matheus e Gustavo
 */
#include "nextion_control_events.h"
#include "nextion_events.h"
#include <stdio.h>

//nesse arquio esta toda a logica de funcionamento das funcoes delcaradas no arquivo HEADER, ira interpretar os evntos
//recebidos do nextion e ira decidir qual decisrao ira tomar

EstadoSistema_t EstadoAtual;
Nextion_EventQueue_t EventQueue;

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
void Nextion__controle_eventos_init(void)
{
	EstadoAtual = STATE_PADRAO;
}

//a funcao abaixo de controle deve ser chamada no loop, eela tenta pegar um evneot da fila (event_dequeue)
//se nao existir retrorna
//se existir, manda para o manipulador correto dependendo do estado atual
void Nextion_controle_eventos_run(void)
{
	//tentando buscar evento na fila circular
	Nextion_event_t* evento = event_dequeue(&EventQueue);

	//se nao houver um evento
	if (evento == NULL){
		return;
	}

	//agora sim iremos passar o evneto para o handler correto
	switch (EstadoAtual)
	{
	case STATE_PADRAO:
		nextion_handle_estado_padrao(evento);
		break;

	case STATE_ESPERA_SP:
		nextion_handle_estado_sp(evento);
		break;

	case STATE_ESPERA_KP:
		nextion_handle_estado_kp(evento);
		break;

	case STATE_CONTROLE_DRIVER:
		nextion_handle_estado_driver(evento);
		break;

	case STATE_CONTROLE_AQUECEDOR:
		nextion_handle_estado_aquecedor(evento);
		break;

	case STATE_CONTROLE_FAN:
		nextion_handle_estado_fan(evento);
		break;

	case STATE_PAGINA_MANUAL:
		nextion_handle_estado_page_manual(evento);
		break;

	case STATE_PAGINA_AUTOMATICO:
		nextion_handle_estado_page_automatico(evento);
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
		break;

	default:
		EstadoAtual = STATE_PADRAO;
		break;
	}
}

//driver
static void nextion_handle_estado_driver(Nextion_event_t *evento){
	if (evento->event == EVENT_TOGGLE_DRIVER){

	}
	EstadoAtual = STATE_PADRAO;
}

//aquecedor
static void nextion_handle_estado_aquecedor(Nextion_event_t *evento){
    switch(evento->event)
    {
        case EVENT_TOGGLE_HEATER:
            // HeaterEnabled = !HeaterEnabled;    esse é um exmplo do qe vamos ter que passar aqui depois, NAO ESQUECER DE FAZER ISSO
            break;

        case EVENT_CONFIRM_HEATER_VALUE:
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
		break;

	case EVENT_CONFIRM_FAN_VALUE:
		break;

	default:
		break;
	}
	EstadoAtual = STATE_PADRAO;
}

//pagina manual
static void nextion_handle_estado_page_manual(Nextion_event_t *evento){
	switch(evento->event)
	{
	case EVENT_TOGGLE_HEATER:
		break;

	case EVENT_TOGGLE_FAN:
		break;

	case EVENT_CONFIRM_HEATER_VALUE:
		break;

	case EVENT_CONFIRM_FAN_VALUE:
		break;

	case EVENT_PAGE_AUTO:
		EstadoAtual = STATE_PAGINA_AUTOMATICO;
		break;

	default:
		break;
	}
}

//pagina automatico
static void nextion_handle_estado_page_automatico(Nextion_event_t *evento){
	switch(evento->event)
	{
	case EVENT_CONFIRM_SP:
		break;

	case EVENT_CONFIRM_KP:
		break;

	case EVENT_PAGE_MANUAL:
		EstadoAtual = STATE_PAGINA_MANUAL;
		break;

	default:
		break;
	}
}
