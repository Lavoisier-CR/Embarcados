# II Avaliação de Sistemas Embarcados com ESP32-S3

Projeto desenvolvido utilizando **ESP32-S3**, **ESP-IDF** e **FreeRTOS** para controle da frequência de um LED através da leitura de um potenciômetro.

## Funcionalidades

* Leitura de potenciômetro via ADC
* Filtro de média móvel com 8 amostras
* Controle da frequência do LED entre 1 Hz e 10 Hz
* Ao acionar o Botão, deve-se acionar os dois modos de operação:

  * Modo 1: Duty Cycle de 50%
  * Modo 2: Duty Cycle de 20%
* Troca de modo através de interrupção externa
* Monitoramento via Serial

## Hardware Utilizado

* ESP32-S3
* Potenciômetro 10 kΩ
* LED
* Resistor 220 Ω
* Botão
* Resistor Pull-up 10 kΩ

## Ligações
 
 * Potenciômetro  GPIO1  
 * LED            GPIO18 
 * Botão          GPIO4  
