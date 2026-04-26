<h1>Atividade 06:</h1>
<h2>Controle de Eventos Críticos com Interrupções e Temporizadores</h2>

## Objetivo

Refatorar o sistema da Atividade 05 para que o processador não precise "vigiar" o botão o tempo todo. O sistema deve ser despertado apenas quando o evento físico ocorrer, utilizando Interrupções de Hardware (ISR).  
Realizar o tratamento do efeito *bounce* por software.

---

## Material Necessário

- 1 ESP32 (DevKit)  
- 1 LED + 1 Resistor de 220 Ω  
- 1 Botão + 1 Resistor de 10 kΩ (opcional, se usar pull-up interno)  
- Protoboard e cabos  

---

## Passos para a Atividade

### Desenvolvimento do firmware

Desenvolva um programa utilizando o ESP-IDF para implementar um controlador de acionamento de um LED.

---

### Requisitos

- **Primeiro Clique:** Liga o LED e inicia a contagem (10 segundos).  

- **Cliques Subsequentes:** Se o LED já estiver ligado, cada novo clique do botão deve reiniciar o temporizador de 10 segundos (como um sensor de presença de corredor que renova o tempo ao detectar movimento).  

- **Desligamento Forçado:** Se o usuário segurar o botão por mais de 2 segundos, o LED deve apagar imediatamente, ignorando o temporizador.

🔌 [Acessar circuito no Wokwi](https://wokwi.com/projects/461968299030152193)
