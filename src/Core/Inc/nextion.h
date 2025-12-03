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


/* FIM DEFINES PARA COMPONTENTES/COMANDOS DO NEXTION */

#endif /* NEXTION_H */

