/*
 * nextion.h
 *
 *  Created on: Oct 31, 2025
 *      Author: julio
 */

#ifndef NEXTION_H
#define NEXTION_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "nextion_events.h"

// Estrutura para dados do touch event
typedef struct
{
	uint8_t page_id;
	uint8_t component_id;
	uint8_t event_type;
} nextion_touch_event_t;


typedef struct
{
	int8_t active_page;
	nextion_touch_event_t touch_event;
}nextion_ihm_t;

//variaveis globais
extern nextion_ihm_t nextion_ihm;


void nextion_init();
void nex_send_cmd(const char *cmd);
void nextion_get_active_page();
void nextion_parse_command(uint8_t *data, uint16_t size);
// Funções auxiliares para interface com o Nextion
void nextion_set_page(uint8_t page_id);
void nextion_set_component_color(const char *component, uint32_t value);
void nextion_set_component_value(const char *component, uint32_t value);
void nextion_set_component_text(const char *component, const char *text);
void nextion_move_component(const char *component_name, int16_t x, int16_t y);
void nextion_set_background_color(const char *page_name, uint32_t value);
//Funções get nextion
void nextion_get_component_value(const char *component_name);

/* DEFINES PARA COMPONTENTES/COMANDOS DO NEXTION */

#define OPERACAO_EVENTO_TOQUE 			0x65
#define OPERACAO_NUMERO_PAGINA 			0x66
#define OPERACAO_RETORNO_VALOR_NUMERICO 0x71

// Defines das mensagens relativas a eventos (criados, não do padrão NEXTION)

#define MSG_NEXTION_ENVIO_VALOR_SP 	  0x10 			/* Nextion envia evento de confirmação do valor do Set Point + Valor setado */
#define MSG_NEXTION_ENVIO_VALOR_KP 	  0x20			/* Nextion envia evento de confirmação do valor do Kp (ganho proporcional) + Valor setado */
#define MSG_NEXTION_ESTADO_DRIVER  	  0x50			/* Nextion envia evento de botão do driver pressionado + novo estado botão */
#define MSG_NEXTION_ESTADO_HEATER  	  0x55			/* Nextion envia evento de botão do heater pressionado + novo estado botão */
#define MSG_NEXTION_ESTADO_FAN	   	  0x56			/* Nextion envia evento de botão do FAN pressionado + novo estado botão */
#define MSG_NEXTION_PAGINA_MANUAL  	  0xF0			/* Nextion informa que a página do display foi trocada para o modo MANUAL */
#define MSG_NEXTION_PAGINA_AUTOMATICO 0xF1			/* Nextion informa que a página do display foi trocada para o modo AUTOMATICO */
#define MSG_NEXTION_DT_HEATER	      0x05			/* Nextion envia o valor setado de duty cicle do heater do usuário para o STM32 */
#define MSG_NEXTION_DT_FAN	      	  0x04			/* Nextion envia o valor setado de duty cicle do fan do usuário para o STM32 */

#define NEXTION_OBJNAME_PV "PV"

/* FIM DEFINES PARA COMPONTENTES/COMANDOS DO NEXTION */

#endif /* NEXTION_H */

