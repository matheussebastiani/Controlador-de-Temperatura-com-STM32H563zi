# Controlador de Temperatura com STM32H563zi
Trabalho final da disciplina de Arquitetura e Programação de Microcontroladores

> Aluno: Matheus Sebastiani Silva - 198204

 Documentação de referência para este README: [Especificação do Projeto](docs/ECP171___Projeto_Final.pdf).

## Objetivo do trabalho

Projetar um **sistema de controle de temperatura** capaz de aumentar e reduzir a temperatura de um ambiente com base na:

- **Temperatura medida**;
- **Valor programado pelo usuário _(set-point)_**.

### Hardware Utilizado:

- Microcontrolador [_STM32H56ZI_](https://www.st.com/en/microcontrollers-microprocessors/stm32h563zi.html) utilizando o kit de desenvolvimento [NUCLEO-H563ZI](https://www.st.com/en/evaluation-tools/nucleo-h563zi.html);
    - Periféricos do Microcontrolador a serem utilizados:
    
    - **ADC** (_Analog-to-digital converter_) para a leitura de um sensor de temperatura LM35;
    
    - **PWM** utilizando os timers internos do microcontrolador para controlar proporcionalmente o aquecedor e o ventilador;

    - **GPIOs** para acender e apagar LEDs pré definidos.

### Requisitos de Firmware:

- O "firmware" deverá ser modular, implementado na Linguagem C utilizando a _HAL_ (_Hardware Abstraction Layer_) do Microcontrolador e utilizando a STM32CubeIDE, separados em vários arquivos `.h` e `.c`.
- O firmware deverá possuir uma **Máquina de Estados** para organizar os modos de operação do hardware. Os estados deverão ser:
    - Automático;
    - Manual;
    - Segurança.

---

## Interface Homem Máquina (IHM)

A interface entre Humano-Microcontrolador deverá obrigatoriamente utilizar um display **Nextion NX8048P070 (7”)**. O projeto de interface deverá ser desenvolvido do zero pelo aluno.

### Requisitos mínimos da IHM

O display deverá apresentar no mínimo as seguintes informações:

- **Temperatura atual** - (**_PV_** - _Process Value_);
- **Temperatura desejada** - (**_SP_** - _Set Point_);
- Estado atual das saídas:
    - Aquecedor (**ON/OFF**);
    - Ventilador (**ON/OFF**);
    - Driver geral: **Habilitado/Desabilitado**.

Através do IHM, deverá ser possível:
- Ajustar um novo **set-point** de temperatura;
- Ajustar o **ganho proporcional** do controlador (**Kp**);
- Habilitar/Desabilitar o controlador, desligando ou ligando o controlador independentemente do valor de temperatura;
- Ligar/Desligar manualmente o aquecedor;
- Ajustar o valor da saída do aquecedor no modo manual (**0-100%**);
- Ligar/Desligar manualmente o ventilador;
- Ajustar o valor da saída do ventilador no modo manual (**0-100%**);

A comunicação entre o microcontrolador e o display da Nextion deverá ser feita via UART, utilizando o protocolo de comandos do NEXTION.

> [!NOTE]
> O Display da NEXTION possui um protocolo próprio para a alteração de textos, variáveis e envio de leituras para o microcontrolador via serial.





