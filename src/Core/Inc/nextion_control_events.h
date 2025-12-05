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

/* Defines de constantes de inicialização */

#define LIGADO 	  1
#define DESLIGADO 0

#define PAGINA_AUTOMATICO 	  	 0
#define PAGINA_MANUAL 	  	  	 1

#define VALOR_INICIAL_SP 	 	-1
#define VALOR_INICIAL_KP 	 	-1
#define VALOR_INICIAL_PV 	 	-1
#define VALOR_INICIAL_ERRO		-1
#define PAGINA_INICIAL 	  	  	 PAGINA_AUTOMATICO
#define ESTADO_INICIAL_DRIVER 	 DESLIGADO
#define ESTADO_INICIAL_AQUECEDOR DESLIGADO
#define ESTADO_INICIAL_FAN 		 DESLIGADO
#define DT_PWM_INICIAL_AQUECEDOR 0
#define DT_PWM_INICIAL_FAN 		 0

#define MODO_SEGURANCA_INICIAL 	 DESLIGADO

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

//variavel global (extern) que mantem o estado atual‍
extern EstadoSistema_t EstadoAtual;

/* Estrutura de controle principal do sistema, conterá todas as informações críticas de controle */

typedef struct{

	int32_t sp;				/* Set Point enviado pelo Nextion	*/
	int32_t kp;				/* Ganho proporcional enviado pelo Nextion	*/
	int8_t pag_atual;   	/* Indica qual é a página atual do Nextion.
							   Através daqui a camada superior saberá o estado de operação do sistema	*/

	int32_t pv;				/* Process Value. Lido nas camadas superiores */
	uint8_t driver_on;		/* Estado do Driver, se está ligado ou desligado */

	uint8_t heater_on;		/* Indica se o aquecedor está ligado ou desligado	*/
	uint8_t fan_on;			/* Indica se o ventilador está ligado ou desligado */

	int8_t fan_dt;			/* Porcentagem informada no display para o PWM do FAN no modo manual */
	int8_t heater_dt;		/* Porcentagem informada no display para o PWM do heater no modo manual */

	int8_t modo_seguranca; 	/* Indica se o modo de segurança está setado ou não. Será utilizado apenas na transição de modos de operação	*/

	int8_t erro;			/* Indica a diferença entre SP e PV	*/

} Controlador_Temp_Infos_t;

extern Controlador_Temp_Infos_t InfosControlador;

//função de inicializacao da pagina, deve ser chamada na inicialiacao do sistema
void Nextion_controle_eventos_init(void);

//função que ira fazer a lógica de fato, verificar se tem algum evento na fila circular de eventos,
//intrepreta esee evento com base no estado atual caso ele exista
//executa acoes e muda o estado se for preciso
void Nextion_controle_eventos_run(void);



#endif /* INC_NEXTION_CONTROL_EVENTS_H_ */
