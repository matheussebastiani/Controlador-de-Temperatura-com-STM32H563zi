/*
 * nextion_events.h
 *
 *  Created on: Dec 3, 2025
 *      Author: matheus, gustavo
 */

#ifndef NEXTION_EVENTS_H_
#define NEXTION_EVENTS_H_

#include <stdint.h>
#include <stdbool.h>

/* Para mapearmos os eventos de maneira simplificada, enviando para camadas superiores o evento que ocorreu,
 * utilizaremos um enum dos eventos possíveis
 */

/* Cola para a hora que o professor perguntar o que é um enum e o motivo de termos usado:
 *
 * Respostas: a) Porquê o ChatGPT fez assim e a gente deu CTRL C + CTRL + V;
 * 			  b) O enum é uma forma de definirmos nomes para constantes numéricas inteiras (iniciando em 0 e incrementando de 1 em 1,
 * 			  deixando o código mais legível e em geral melhor do que utilizar vários #defines para todas as constantes;
 * 			  É possível utilizar o typedef com o enum, assim como nas structs. O typedef elimina a necessidade de utilizarmos o enum
 * 			  toda vez que o "instanciarmos". No caso, cada evento será um enum.
 * 			  Esse enum vai ser passado para as camadas superiores
 * 	*/

typedef enum {

	EVENT_NONE, 		// Nada aconteceu
	EVENT_CONFIRM_SP,
	EVENT_CONFIRM_KP,
	EVENT_TOGGLE_DRIVER,
	EVENT_TOGGLE_HEATER,
	EVENT_TOGGLE_FAN,
	EVENT_CONFIRM_HEATER_VALUE,
	EVENT_CONFIRM_FAN_VALUE,
	EVENT_PAGE_MANUAL,
	EVENT_PAGE_AUTO

} Nextion_events_type_t;

// Como todos os eventos estão atrelados a um valor, podemos fazer uma struct contendo o valor, e o evento ocorrido

typedef struct {

	Nextion_events_type_t event;
	int32_t value;

} Nextion_event_t;

extern Nextion_event_t NextionEvent; // Essa struct será utilizada por outras partes do código


/* FILA DE EVENTOS:
 * Para garantir que nenhum evento do Nextion seja perdido por conta de duas interrupções muito próximas na UART, sendo
 * a primeira não consumida/processada pela camada superior, faremos um sistema de FILA. Ou seja, o evento chega, é enfileirado
 * nessa camada, e é consumido pelo loop principal de controle
 */

#define TAMANHO_MAXIMO_FILA 32

typedef struct {

	Nextion_event_t event_queue[TAMANHO_MAXIMO_FILA];
	int first;
	int last;

} Nextion_EventQueue_t;

/* Enquanto observamos a fila, é necessário lembrar de um princípio FUNDAMENTAL
 	 	 Veja que:
 	 * Essa fila será consumida pelo laço de controle principal.
 	 * E será alimentada por uma função chamada por um callback de uma INTERRUPÇÃO!!!!!
 	 * Ou seja, CONCORRENCIA. A FILA DE EVENTOS É UMA SEÇÃO CRÍTICA!
 	 * Para evitar valores inconsistentes no caso de ocorrer uma interrupção na UART exatamente na hora que o
 	 * laço principal de controle for consumir algo da fila, é necessário garantir exclusão mútua da fila para o laço principal
 	 * Como não estamos utilizando nenhum tipo de Sistema Operacional, que provê mecanismos de sincronização de alto nível,
 	 * a única possibilidade para garantir exclusão mútua é através do desligamento temporário das interrupções para a CPU.
 	 * Ou seja, vamos desligar as interrupções quando acessarmos a fila, e ligar novamente após utilizá-la.
 	 * Isso garante que nenhuma interrupção de periférico desvie o Program Count (ou IP) para um tratador de interrupções que mexerá
 	 * nos valores de queue->first e queue->last.
 	 * A ARM provê um mecanismo de desligamento de interrupções através da função __disable_irq();
 	 * Para ligar novamente, é necessário utilizar a função __enable_irq();
 */

void event_queue_init(Nextion_EventQueue_t *queue);

int event_enqueue(Nextion_EventQueue_t *queue,Nextion_event_t *evento);

Nextion_event_t* event_dequeue(Nextion_EventQueue_t *queue);

bool isQueueFull(Nextion_EventQueue_t *queue);

bool isQueueEmpty(Nextion_EventQueue_t *queue);


#endif /* NEXTION_EVENTS_H_ */
