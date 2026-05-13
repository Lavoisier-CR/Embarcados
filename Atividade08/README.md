<div align="center">
   <img src="https://www.ifpb.edu.br/imagens/logotipos/campina-grande/@@images/image-1200-119374a47048af0ba09197e64453797c.png" width="150px">
</div>

### Engenharia de Computação  
### Sistemas Embarcados  
#### Professor: Alexandre Sales Vasconcelos  
>**Aluno:** &emsp;Lavoisier Chaves Ramos

<div align="center">
   <h3> Atividade 08 </h3>  
   <h4>Monitoramento Analógico e Controle de PWM</h4>
</div>

#### Objetivo:
Compreender o funcionamento do periférico ADC no ESP32 para converter tensões analógicas (0V a 3.3V) em valores digitais (0 a 4095). Implementar o escalonamento desses dados para controlar o brilho de um LED via PWM (LEDC) e utilizar um botão para "congelar" a leitura.
#### Material Necessário:

* 1 ESP32 (DevKit)
* 1 Potenciometro (10 kΩ)
* 1 LED + 1 Resistor de 220 Ω
* 1 Button + 1 Resistor  10 kΩ
* Protoboard e cabos

#### Passos para a Atividade:

1. `Esquemático:`

* Elabore o circuito esquemático do dispositivo que atenda os requisitos apresentados no item 3.

2. `Parâmetros de configuração dos periféricos:`

* Apresente os valores de configuração para os principais componentes/periféricos utilizados no desenvolvimento do dispositivo (Utilize os materiais de aula para escolher os parâmetros).

* Apresente também a equação utilizada para converter o valor bruto do ADC em tensão.

3. `Desenvolvimento do firmware:`
   
* Desenvolva um programa utilizando o ESP-IDF que utilize a ADC para realizar as seguintes tarefas:

#### `Requisitos:`

* `ADC:` Configure o ADC para a leitura de uma ddp máxima de 3,3V com os valores apresentados no item 2.
  
* `Modo de Operação:` O brilho do LED deve ser proporcional à posição do potenciômetro (utilize o driver ledc para gerar PWM).

* `Função "Hold" (Botão):` A cada acionamento do botão, o sistema deve entrar em modo HOLD. Nesse estado, o brilho do LED fica travado no último valor lido e o sistema para de atualizar a leitura do potenciômetro. Um novo acionamento do botão, o sistema volta a ler o potenciômetro em tempo real. 

* `Monitoramento:` Imprima no terminal a cada 500ms: o valor bruto do ADC, a tensão calculada (em mV) e o estado atual (LIVE ou HOLD).
