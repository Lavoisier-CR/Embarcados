<img src="https://www.ifpb.edu.br/static/media/logo.20d47050.png" width="150px">

# Engenharia de Computação
## Sistemas Embarcados
### Professor: Alexandre Sales Vasconcelos
>**Equipe:** &emsp;Lavoisier Chaves Ramos\
>&emsp;&emsp;&emsp;&emsp; Vinícius Cavalcante Barbosa

# Atividade 07
<h2>Comunicação Serial UART com Teste de Loopback</h2>
<h3>Objetivo:</h3>
Compreender o funcionamento da interface UART (Universal Asynchronous Receiver-Transmitter) no ESP32. Implementar o envio e recebimento de dados de forma assíncrona, utilizando um jumper físico para criar um canal de "auto-eco" (Loopback).
<h3>Material Necessário:</h3>

* 1 ESP32 (DevKit)
* 1 LED + 1 Resistor de 220 Ohm
* 1 Jumper (para fechar o curto entre TX e RX)
* Protoboard e cabos
<h3>Passos para a Atividade:</h3>

**Desenvolvimento do firmware:** Desenvolva um programa utilizando o ESP-IDF que utilize a UART2 para realizar as seguintes tarefas:

<h3>Requisitos:</h3>

* **Configuração UART:** Configure a UART2 com Baud Rate de **115200**, 8 bits de dados, 1 stop bit e sem paridade.
* **Envio Periódico:** A cada **2 segundos**, o ESP32 deve enviar uma string fixa pela UART (Ex: "LIGAR" ou "DESLIGAR").
* **Processamento de Recepção:** O sistema deve "escutar" a própria mensagem enviada (via jumper).
* Ao receber a string **"LIGAR"**, o LED deve acender.
* Ao receber a string **"DESLIGAR"**, o LED deve apagar.
* **Feedback no Monitor:** Imprima no console principal (UART0) o que foi enviado e o que foi efetivamente recebido para validar a integridade dos dados.
