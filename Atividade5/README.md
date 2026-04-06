<h1>Atividade 05:</h1>
<h2>Sistema de Iluminação com Temporizador</h2>

<h3>Objetivo:</h3>

Implementar o controle de um LED via botão utilizando a técnica de polling e controle de tempo via software, garantindo que o sistema permaneça responsivo.
Realizar o tratamento do efeito bounce por software.

<h4>Material Necessário:</h4>

* 1 ESP32 (DevKit)
* 1 LED + 1 Resistor de 220 &#937;
* 1 Botão + 1 Resistor de 10 k&#937; (opcional, se usar Pull-up interno)
* Protoboard e cabos

<h4>Passos para a Atividade:</h4>

* Elaborar o diagrama em bloco

* Elaborar o esquemático.

* Desenvolvimento do Código:

<h5>Desenvolva um programa utilizando o ESP-IDF para implementar um controlador de acionamento de um LED:</h5>

* **Leitura Digital:** O programa deve ler o estado do botão.
* **Toggle Simples:** Ao pressionar o botão, o LED deve mudar de estado (se estava aceso, apaga; se estava apagado, acende).
* **Temporizador de Segurança:** Se o LED for ligado e o usuário esquecer de desligar, o sistema deve apagá-lo automaticamente após **10 segundos**.
