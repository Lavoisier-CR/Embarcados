#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

#include "esp_log.h"
#include "esp_timer.h"

#include "esp_adc/adc_oneshot.h"

#define TAG "ADC_PWM"

// =====================================================
// DEFINIÇÕES
// =====================================================
#define ADC_CHANNEL ADC_CHANNEL_5   // GPIO6
#define BUTTON_GPIO GPIO_NUM_7
#define LED_GPIO    GPIO_NUM_18

// =====================================================
// VARIÁVEIS GLOBAIS
// =====================================================
volatile bool interrupt_flag = false;

bool hold_mode = false;

// ADC Handle
adc_oneshot_unit_handle_t adc_handle;

// =====================================================
// ISR DO BOTÃO
// =====================================================
void IRAM_ATTR button_isr_handler(void *arg)
{
  // Apenas sinaliza evento
  interrupt_flag = true;
}

// =====================================================
// ADC INIT
// =====================================================
void adc_init()
{
  adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT_1,
  };

    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t config = {
      .bitwidth = ADC_BITWIDTH_12,
      .atten = ADC_ATTEN_DB_12,
    };

    adc_oneshot_config_channel(adc_handle,
                               ADC_CHANNEL,
                               &config);
}

// =====================================================
// PWM INIT
// =====================================================
void pwm_init()
{
  ledc_timer_config_t timer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .duty_resolution = LEDC_TIMER_12_BIT,
    .freq_hz = 5000,
    .clk_cfg = LEDC_AUTO_CLK
  };

  ledc_timer_config(&timer);

  ledc_channel_config_t channel = {
    .gpio_num = LED_GPIO,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
  };

  ledc_channel_config(&channel);
}

// =====================================================
// BUTTON INIT
// =====================================================
void button_init()
{
  gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << BUTTON_GPIO),
    .mode = GPIO_MODE_INPUT,

    // pull-up EXTERNO
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,

    // botão pressionado = HIGH -> LOW
    .intr_type = GPIO_INTR_NEGEDGE
  };

  gpio_config(&io_conf);

  gpio_install_isr_service(0);

  gpio_isr_handler_add(BUTTON_GPIO,
                         button_isr_handler,
                         NULL);
}

// =====================================================
// MAIN
// =====================================================
void app_main()
{
  adc_init();

  pwm_init();

  button_init();

  int adc_raw = 0;

  int duty = 0;

  int last_duty = 0;

  while (1)
    {
      // =================================================
      // TRATAMENTO DO BOTÃO + DEBOUNCE
      // =================================================
      if (interrupt_flag)
        {
          // espera bounce passar
          vTaskDelay(pdMS_TO_TICKS(50));

          // confirma se botão ainda está pressionado
          if (gpio_get_level(BUTTON_GPIO) == 0)
            {
              // alterna estado
              hold_mode = !hold_mode;

              if (hold_mode)
                {
                  ESP_LOGI(TAG,"======== HOLD ATIVADO ========");
                }
              else
                {
                  ESP_LOGI(TAG,"======== HOLD DESATIVADO ========");
                }

              // espera soltar botão
              while (gpio_get_level(BUTTON_GPIO) == 0)
                {
                  vTaskDelay(pdMS_TO_TICKS(10));
                }
            }

          interrupt_flag = false;
        }

      // =================================================
      // MODO NORMAL
      // =================================================
      if (!hold_mode)
        {
          adc_oneshot_read(adc_handle,
                            ADC_CHANNEL,
                            &adc_raw);

          int voltage = (adc_raw * 3300) / 4095;

          // =============================================
          // CURVA GAMA - AJUSTA PERCEPÇÃO DE BRILHO
          // =============================================
          float normalized = (float)adc_raw / 4095.0;

          float gamma = 2.2;

          duty = (int)(pow(normalized, gamma) * 4095.0);

          last_duty = duty;

          ESP_LOGI(TAG,
                    "ADC: %d | Tensão: %d mV | PWM: %d",
                    adc_raw,
                    voltage,
                    duty);
        }
      else
        {
          // mantém último valor
          duty = last_duty;
        }

      // =================================================
      // ATUALIZA PWM
      // =================================================
      ledc_set_duty(LEDC_LOW_SPEED_MODE,
                    LEDC_CHANNEL_0,
                    duty);

      ledc_update_duty(LEDC_LOW_SPEED_MODE,
                        LEDC_CHANNEL_0);

      vTaskDelay(pdMS_TO_TICKS(100));
    }
}
