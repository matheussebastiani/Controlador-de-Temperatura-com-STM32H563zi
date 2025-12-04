/*
 * nextion_control_events.h
 *
 *  Created on: Dec 4, 2025
 *      Author: Matheus e Gustavo
 */

#ifndef INC_NEXTION_CONTROL_EVENTS_H_
#define INC_NEXTION_CONTROL_EVENTS_H_

/*
 * Arquivo: nextion_state_machine.h
 * --------------------------------
 * Este arquivo define a máquina de estados responsável por interpretar
 * os eventos vindos do Nextion (via UART) e transformar esses eventos
 * em ações internas do sistema.
 *
 * O Nextion envia comandos → os comandos viram eventos na fila →
 * a máquina de estados interpreta cada evento e toma decisões.
 *
 * Exemplos de ações tomadas pela máquina:
 *   - Atualizar SP (setpoint)
 *   - Atualizar KP do controle
 *   - Ligar/desligar heater, fan ou driver
 *   - Mudar página entre manual/automático
 *
 * Este arquivo contém:
 *   - Enum dos estados possíveis
 *   - Variável global com o estado atual
 *   - Funções de inicialização e execução da máquina
 */

#include <stdint.h>
#include <stdbool.h>
#include "nextion_events.h" //acessar a fila de evntos

// estados rpesentes na maquina para controle dos eventos
typedef enum {
	STATE_PADRAO,
	STATE_ESPERA_SP,
	STATE_ESPERA_KP,
	STATE_CONTROLE_DRIVER,
	STATE_CONTROLE_AQUECEDOR,
	STATE_CONTROLE_FAN,
	STATE_PAGINA_MANUAL,
	STATE_PAGINA_AUTOMATICO
} EstadoSistema_t;


//vairavel global (extern) que mantem o estado atual
extern EstadoSistema_t EstadoAtual;

//função de inicializacao da pagina, deve ser chamada na inicialiacao do sistema
void Nextion_controle_eventos_init(void);

//função que ira fazer a lógica de fato, verificar se tem algum evento na fila circular de eventos,
//intrepreta esee evento com base no estado atual caso ele exista
//executa acoes e muda o estado se for preciso
void Nextion_controle_eventos_run(void);


#endif /* INC_NEXTION_CONTROL_EVENTS_H_ */
