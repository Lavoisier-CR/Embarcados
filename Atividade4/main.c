#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define BTN_A GPIO_NUM_10
#define BTN_B GPIO_NUM_11

#define LED0 GPIO_NUM_4
#define LED1 GPIO_NUM_5
#define LED2 GPIO_NUM_6
#define LED3 GPIO_NUM_7

#define DEBOUNCE_TIME 50000 // 50 ms

uint8_t contador = 0;
uint8_t passo = 1;

int64_t last_press_a = 0;
int64_t last_press_b = 0;

int estado_a_anterior = 1;
int estado_b_anterior = 1;

void atualizar_leds(uint8_t valor) {
    gpio_set_level(LED0, (valor >> 0) & 1);
    gpio_set_level(LED1, (valor >> 1) & 1);
    gpio_set_level(LED2, (valor >> 2) & 1);
    gpio_set_level(LED3, (valor >> 3) & 1);
}

void app_main(void)
{
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL<<LED0) | (1ULL<<LED1) |
                        (1ULL<<LED2) | (1ULL<<LED3)
    };
    gpio_config(&io_conf);

    gpio_config_t btn_conf = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pin_bit_mask = (1ULL<<BTN_A) | (1ULL<<BTN_B)
    };
    gpio_config(&btn_conf);

    atualizar_leds(contador);

    while (1) {
        int64_t now = esp_timer_get_time();

        int estado_a = gpio_get_level(BTN_A);
        int estado_b = gpio_get_level(BTN_B);

        if (estado_a == 0 && estado_a_anterior == 1) {
            if (now - last_press_a > DEBOUNCE_TIME) {
                contador = (contador + passo) & 0x0F;
                atualizar_leds(contador);
                last_press_a = now;
            }
        }

        if (estado_b == 0 && estado_b_anterior == 1) {
            if (now - last_press_b > DEBOUNCE_TIME) {
                passo = (passo == 1) ? 2 : 1;
                last_press_b = now;
            }
        }

        estado_a_anterior = estado_a;
        estado_b_anterior = estado_b;

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
