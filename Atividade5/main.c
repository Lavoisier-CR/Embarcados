#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

// Definições de Pinos
#define BUTTON_GPIO          8
#define LED_GPIO             4
#define DEBOUNCE_TIME_MS     50
#define AUTO_OFF_TIME_US     10000000 // 10 segundos em microssegundos

void app_main(void) {
    // Configuração do LED
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    // Configuração do Botão (Pull-up interno ativado)
    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);

    int led_state = 0;
    int last_button_state = 1; // 1 = Solto (Pull-up)
    int64_t led_on_time = 0;

    printf("Sistema de Iluminação Iniciado...\n");

    while (1) {
        int current_button_state = gpio_get_level(BUTTON_GPIO);

        // Lógica de Toggle com Debounce Simples e Polling
        if (last_button_state == 1 && current_button_state == 0) {
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_TIME_MS)); // Debounce
            if (gpio_get_level(BUTTON_GPIO) == 0) {
                led_state = !led_state;
                gpio_set_level(LED_GPIO, led_state);
                
                if (led_state) {
                    led_on_time = esp_timer_get_time(); // Marca o tempo que ligou
                    printf("LED Ligado! Temporizador iniciado.\n");
                } else {
                    printf("LED Desligado via botão.\n");
                }
            }
        }
        last_button_state = current_button_state;

        // Temporizador de Segurança (10 segundos)
        if (led_state) {
            int64_t current_time = esp_timer_get_time();
            if ((current_time - led_on_time) >= AUTO_OFF_TIME_US) {
                led_state = 0;
                gpio_set_level(LED_GPIO, led_state);
                printf("Temporizador: LED desligado automaticamente após 10s.\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Pequeno delay para aliviar a CPU (Watchdog)
    }
}
