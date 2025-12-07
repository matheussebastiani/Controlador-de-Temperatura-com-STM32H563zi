/*
 * nextion_control_events.h
 *
 *  Created on: Dec 4, 2025
 *      Author: Matheus e Gustavo
 */

#ifndef INC_NEXTION_CONTROL_EVENTS_H_
#define INC_NEXTION_CONTROL_EVENTS_H_

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

	int32_t fan_dt;			/* Porcentagem informada no display para o PWM do FAN no modo manual */
	int32_t heater_dt;		/* Porcentagem informada no display para o PWM do heater no modo manual */

	int32_t modo_seguranca; 	/* Indica se o modo de segurança está setado ou não. Será utilizado apenas na transição de modos de operação	*/

	int32_t erro;			/* Indica a diferença entre SP e PV	*/

} Controlador_Temp_Infos_t;

extern Controlador_Temp_Infos_t InfosControlador;

//função de inicializacao da pagina, deve ser chamada na inicialiacao do sistema
void Nextion_controle_eventos_init(void);

//função que ira fazer a lógica de fato, verificar se tem algum evento na fila circular de eventos,
//intrepreta esee evento com base no estado atual caso ele exista
//executa acoes e muda o estado se for preciso
void Nextion_controle_eventos_run(void);



#endif /* INC_NEXTION_CONTROL_EVENTS_H_ */
