#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_timer.h"
#include "esp_err.h"

//==================================================
// GPIOs
//==================================================

#define LED_GPIO        GPIO_NUM_18
#define BUTTON_GPIO     GPIO_NUM_4

//==================================================
// ADC
//==================================================

#define ADC_CHANNEL     ADC_CHANNEL_0      // GPIO1
#define ADC_MAX_VALUE   4095

//==================================================
// Filtro de Média Móvel
//==================================================

#define FILTER_SIZE     8

//==================================================
// Variáveis Globais
//==================================================

static volatile uint8_t current_mode = 1;
static volatile int64_t last_interrupt_time = 0;

static float blink_frequency = 1.0f;
static float potentiometer_percent = 0.0f;
static uint16_t filtered_adc = 0;

static adc_oneshot_unit_handle_t adc_handle;

//==================================================
// ISR do Botão
//==================================================

static void IRAM_ATTR button_isr_handler(void *arg)
{
    int64_t now = esp_timer_get_time();

    if ((now - last_interrupt_time) > 200000)
    {
        current_mode++;

        if (current_mode > 2)
        {
            current_mode = 1;
        }

        last_interrupt_time = now;
    }
}

//==================================================
// Task ADC
//==================================================

void adc_task(void *pvParameters)
{
    uint16_t samples[FILTER_SIZE] = {0};
    uint32_t sum = 0;
    uint8_t index = 0;

    float last_print_percent = -100.0f;

    while (1)
    {
        int adc_value = 0;

        ESP_ERROR_CHECK(
            adc_oneshot_read(
                adc_handle,
                ADC_CHANNEL,
                &adc_value
            )
        );

        sum -= samples[index];
        samples[index] = adc_value;
        sum += adc_value;

        index = (index + 1) % FILTER_SIZE;

        filtered_adc = sum / FILTER_SIZE;

        potentiometer_percent =
            ((float)filtered_adc * 100.0f) / ADC_MAX_VALUE;

        blink_frequency =
            1.0f + ((potentiometer_percent / 100.0f) * 9.0f);

        if (fabsf(potentiometer_percent - last_print_percent) >= 5.0f)
        {
            printf("\n====================================\n");
            printf("ADC: %u\n", filtered_adc);
            printf("Percentual: %.0f%%\n", potentiometer_percent);
            printf("Frequencia: %.1f Hz\n", blink_frequency);

            if (current_mode == 1)
            {
                printf("Modo: 1 (Duty Cycle 50%%)\n");
            }
            else
            {
                printf("Modo: 2 (Duty Cycle 20%%)\n");
            }

            printf("====================================\n");

            last_print_percent = potentiometer_percent;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//==================================================
// Task LED
//==================================================

void led_task(void *pvParameters)
{
    while (1)
    {
        float freq = blink_frequency;

        if (freq < 1.0f)
        {
            freq = 1.0f;
        }

        float period_ms = 1000.0f / freq;

        float duty_cycle =
            (current_mode == 1) ? 0.50f : 0.20f;

        uint32_t on_time_ms =
            (uint32_t)(period_ms * duty_cycle);

        uint32_t off_time_ms =
            (uint32_t)(period_ms - on_time_ms);

        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(on_time_ms));

        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(off_time_ms));
    }
}

//==================================================
// app_main
//==================================================

void app_main(void)
{
    //==============================================
    // LED
    //==============================================

    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&led_config));

    //==============================================
    // Botão
    //==============================================

    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };

    ESP_ERROR_CHECK(gpio_config(&button_config));

    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    ESP_ERROR_CHECK(
        gpio_isr_handler_add(
            BUTTON_GPIO,
            button_isr_handler,
            NULL
        )
    );

    //==============================================
    // ADC One-Shot (ESP-IDF 5.x)
    //==============================================

    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };

    ESP_ERROR_CHECK(
        adc_oneshot_new_unit(
            &init_cfg,
            &adc_handle
        )
    );

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12
    };

    ESP_ERROR_CHECK(
        adc_oneshot_config_channel(
            adc_handle,
            ADC_CHANNEL,
            &chan_cfg
        )
    );

    //==============================================
    // Tasks
    //==============================================

    xTaskCreate(
        adc_task,
        "adc_task",
        4096,
        NULL,
        5,
        NULL
    );

    xTaskCreate(
        led_task,
        "led_task",
        4096,
        NULL,
        5,
        NULL
    );

    printf("\nSistema iniciado!\n");
}