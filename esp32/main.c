#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define PWM_PIN        18       
#define PWM_FREQ_HZ    500    
#define PWM_RES_BITS   LEDC_TIMER_11_BIT  
#define PWM_CHANNEL    LEDC_CHANNEL_0
#define PWM_TIMER      LEDC_TIMER_0
#define PWM_MODE       LEDC_HIGH_SPEED_MODE

void pwm_init(void) {
    // 1. Configurar el temporizador
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = PWM_MODE,
        .timer_num        = PWM_TIMER,
        .duty_resolution  = PWM_RES_BITS,
        .freq_hz          = PWM_FREQ_HZ,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // 2. Configurar el canal
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = PWM_MODE,
        .channel        = PWM_CHANNEL,
        .timer_sel      = PWM_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = PWM_PIN,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}

void app_main(void) {
    pwm_init();

    const uint32_t max_duty = (1 << 11) - 1;  
    uint32_t duty = 0;

    while (1) {
        ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
        ledc_update_duty(PWM_MODE, PWM_CHANNEL);

        duty += (max_duty / 5);  
        if (duty > max_duty) {
            duty = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));  // Esperar 
    }
}
