/*
 * nextion_events.c
 *
 *  Created on: Dec 3, 2025
 *      Author: matheus, gustavo
 */

#include "nextion_events.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

Nextion_event_t NextionEvent;
Nextion_EventQueue_t FilaEventos;

/* Implementação relativa a fila de eventos que o algoritmo de controle principal consultará */

/*	Inicia a fila com first e last em -1	*/
void event_queue_init(Nextion_EventQueue_t *queue){

	__disable_irq(); // Desliga interrupções (concorrencia)

	queue->first = -1;
	queue->last = -1;

	__enable_irq(); // Habilita novamente
}

/* Função que enfileira um evento. Como vamos trabalhar com uma fila circular, vamos recusar eventos caso a fila esteja cheia (dificilmente estará)
 * Relembrando a lógica do enqueue:
 * Checa se a fila está cheia (evitar Overflow)
 * Se não estiver cheia, incrementa o last (lembrando que é circular) e insere o evento no final da fila
 */

int event_enqueue(Nextion_EventQueue_t *queue, Nextion_event_t *evento){ // Usado pelo tratador de interrupções, não precisa desabilitar interrupções

	if(isQueueFull(queue)){
		return -1;
	}

	if(queue->first == -1){
		queue->first = 0;
	}

	queue->last = (queue->last + 1) % TAMANHO_MAXIMO_FILA;

	queue->event_queue[queue->last] = *evento;

	return 1;

}

Nextion_event_t* event_dequeue(Nextion_EventQueue_t *queue){ // Utilizada pelo controle principal -> necessário desabilitar interrupções

	Nextion_event_t* evento;

	if(isQueueEmpty(queue)){ // Underflow
		return NULL;
	}

	__disable_irq();

	evento = &queue->event_queue[queue->first];

	if(queue->first == queue->last){ // Significa que agora a fila ficou vazia!!
		queue->first = queue->last = -1;
	} else{ // Caso ainda exista algo na fila, move o first uma casa adiante

		queue->first = (queue->first + 1) % TAMANHO_MAXIMO_FILA;
	}

	__enable_irq();

	return evento;

}

bool isQueueFull(Nextion_EventQueue_t *queue){

	if((queue->last + 1) % TAMANHO_MAXIMO_FILA == queue->first){ // Overflow -> Fila cheia!
		return true;
	}

	return false;

}

bool isQueueEmpty(Nextion_EventQueue_t *queue){

	return queue->first == -1;

}
