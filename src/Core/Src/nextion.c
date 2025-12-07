/*
 * nextion.c
 *
 *  Created on: Oct 31, 2025
 *      Author: julio
 */

#include "nextion.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "nextion_events.h"

/************************************************************************************************/
/**************** Mudar a porta serial utilzada pela ihm nextion aqui!!! ************************/
/************************************************************************************************/

#define NEXTION_UART huart3 // Porta serial virtual do ST-Link, para executar com o simulador Nextion
//#define NEXTION_UART huart2	// Porta Serial Real, para executar com display fisico

/************************************************************************************************/
/************************************************************************************************/

// ajusta a porta serial para o componente escolhido
extern UART_HandleTypeDef NEXTION_UART;
#define NX_UART	(&NEXTION_UART)

// tamanho e declaração do buffer de recepçao
#define NEXTION_RX_BUFFER_SIZE	128
uint8_t nextion_rx_buffer[NEXTION_RX_BUFFER_SIZE];

nextion_ihm_t nextion_ihm;


// inicializa a interface, deve ser chamada antes do envio de comandos.
void nextion_init()
{
	HAL_UARTEx_ReceiveToIdle_IT(NX_UART, nextion_rx_buffer,
			sizeof(nextion_rx_buffer));

	nextion_ihm.active_page = -1;
	nextion_ihm.touch_event.component_id = -1;
	nextion_ihm.touch_event.event_type = -1;
	nextion_ihm.touch_event.page_id = -1;

	NextionEvent.event = -1;
	NextionEvent.value = -1;
}

// envia comandos para a ihm, coloca o terminador ff ff ff de forma automática ao final do comando.
void nex_send_cmd(const char *cmd)
{
	HAL_UART_Transmit(NX_UART, (uint8_t*) cmd, strlen(cmd), HAL_MAX_DELAY);
	const uint8_t t[3] =
	{ 0xFF, 0xFF, 0xFF };
	HAL_UART_Transmit(NX_UART, t, 3, HAL_MAX_DELAY);
    HAL_Delay(1);
}

// solicita qual a pagina está ativa na IHM, a respota chega no callback da porta serial escolhida (HAL_UARTEx_RxEventCallback)
void nextion_get_active_page()
{
	nex_send_cmd("sendme");
}


// quando chegam novos dados na serial, este callback é chamado, os dados são processados, e o callback é reiniciado para permitir novas recepções
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	// garante que a origem foi a ihm
	if (huart->Instance == NEXTION_UART.Instance && Size > 0)
	{
		// Processa os dados recebidos
		nextion_parse_command(nextion_rx_buffer, Size);

		// ⭐⭐ IMPORTANTE: Reinicia a recepção ⭐⭐
		HAL_UARTEx_ReceiveToIdle_IT(NX_UART, nextion_rx_buffer,
				sizeof(nextion_rx_buffer));
	}
}

// Função para processar comandos Nextion
void nextion_parse_command(uint8_t *data, uint16_t size)
{
    if (size < 2)   // mínimo necessário
        return;

    uint8_t command = data[0];
    uint8_t value   = data[1];

    Nextion_event_t ev;
    ev.event = EVENT_NONE;
    ev.value = value;

    switch (command)
    {
        case MSG_NEXTION_ENVIO_VALOR_SP:
            ev.event = EVENT_CONFIRM_SP;
            break;

        case MSG_NEXTION_ENVIO_VALOR_KP:
            ev.event = EVENT_CONFIRM_KP;
            break;

        case MSG_NEXTION_ESTADO_DRIVER:
            ev.event = EVENT_TOGGLE_DRIVER;
            break;

        case MSG_NEXTION_ESTADO_HEATER:
            ev.event = EVENT_TOGGLE_HEATER;
            break;

        case MSG_NEXTION_ESTADO_FAN:
            ev.event = EVENT_TOGGLE_FAN;
            break;

        case MSG_NEXTION_PAGINA_MANUAL:
            ev.event = EVENT_PAGE_MANUAL;
            break;

        case MSG_NEXTION_PAGINA_AUTOMATICO:
            ev.event = EVENT_PAGE_AUTO;
            break;

        case MSG_NEXTION_DT_HEATER:
            ev.event = EVENT_CONFIRM_HEATER_VALUE;
            break;

        case MSG_NEXTION_DT_FAN:
            ev.event = EVENT_CONFIRM_FAN_VALUE;
            break;

        default:
            return; // comando inválido
    }

    event_enqueue(&FilaEventos, ev);
}


// Funções auxiliares para interface com o Nextion
void nextion_set_page(uint8_t page_id)
{
	char cmd[20];
	snprintf(cmd, sizeof(cmd), "page %d", page_id);
	nex_send_cmd(cmd);
}

void nextion_set_component_value(const char *component, uint32_t value)
{
	char cmd[30];
	snprintf(cmd, sizeof(cmd), "%s.val=%lu", component, value);
	nex_send_cmd(cmd);
}

void nextion_set_component_color(const char *component, uint32_t value)
{
	char cmd[30];
	snprintf(cmd, sizeof(cmd), "%s.pco=%lu", component, value);
	nex_send_cmd(cmd);
}

void nextion_set_background_color(const char *page_name, uint32_t value)
{
	char cmd[30];
	snprintf(cmd, sizeof(cmd), "%s.bco=%lu", page_name, value);
	nex_send_cmd(cmd);
}

void nextion_set_component_text(const char *component, const char *text)
{
	char cmd[50];
	snprintf(cmd, sizeof(cmd), "%s.txt=\"%s\"", component, text);
	nex_send_cmd(cmd);
}

// Mover componente para posição específica (ambas coordenadas em um comando)
void nextion_move_component(const char *component_name, int16_t x, int16_t y)
{
    char cmd[50];
    snprintf(cmd, sizeof(cmd), "%s.x=%u", component_name, x);
    nex_send_cmd(cmd);
    snprintf(cmd, sizeof(cmd), "%s.y=%u", component_name, y);
    nex_send_cmd(cmd);
}

void nextion_get_component_value(const char *component_name){

	char cmd[30];
	snprintf(cmd, sizeof(cmd), "get %s.val", component_name);
	nex_send_cmd(cmd);
}
