/*
 * nextion_interface.h
 *
 *  Created on: Dec 5, 2025
 *      Author: Matheus Gustavo
 */

#ifndef INC_NEXTION_INTERFACE_H
#define INC_NEXTION_INTERFACE_H

#include <stdint.h>
#include "nextion_control_events.h"

/*
 * Arquivo responsável por FAZER A PONTE FINAL entre o firmware e o Nextion.
 *
 * Aqui colocamos funções de ALTO NÍVEL para:
 *   - atualizar valores na tela
 *   - trocar de página
 *   - atualizar estados (driver, heater, fan)
 *   - enviar textos para status
 *
 * AQUI NÃO interpretamos eventos.
 * SOMENTE enviamos comandos para a tela.
 */

void nextion_atualiza_pv(int32_t pv);                    //atualiza o valor do process value
void nextion_atualiza_kp(int32_t kp);                    //atualzia o valor da varialvel kp
void nextion_atualiza_sp(int32_t sp);                    //atualiza o valor do set point

void nextion_atualiza_driver(uint8_t driver_on);				 //atualiza o estado do driver
void nextion_atualiza_fan(uint8_t fan_on);		             //atualiza o estado do fan e o duty cycle
void nextion_atualiza_aquecedor(uint8_t heater_on);             //atualiza o estado do aquecedor
void nextion_atualiza_aquecedor_dt(int8_t heater_dt);			 //atualiza o duty cycle do aquecedpr
void nextion_atualiza_fan_dt(int8_t fan_dt);                 //atualiza o duty cycle do fan


//essas duas funcoes naop estao sendo usadas no momento pois o tratamento dos componentes e sua inicializacao ja esta sendo feita no case de cada pagina, mas é possivel mudar a logica para usar as funcoes abaixo
void nextion_goto_pagina_manual(void);					 //vai para a pagina manual
void nextion_goto_pagina_automatico(void);               //vai para a pagina automatico

void nextion_status(const char* status);                 //apena status de controle, nao sei se vamos usar ainda, mas esta aqui por enquanto

#endif /* INC_NEXTION_INTERFACE_H */
