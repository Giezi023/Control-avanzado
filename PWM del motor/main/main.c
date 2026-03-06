#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define PWM_PIN 5
#define PWM_CHANNEL LEDC_CHANNEL_0
#define PWM_TIMER LEDC_TIMER_0
#define PWM_MODE LEDC_HIGH_SPEED_MODE
#define PWM_FREQ 5000
#define PWM_RES LEDC_TIMER_8_BIT

void app_main(void)
{
    ledc_timer_config_t pwm_timer = {
        .speed_mode = PWM_MODE,
        .timer_num = PWM_TIMER,
        .duty_resolution = PWM_RES,
        .freq_hz = PWM_FREQ,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&pwm_timer);

    ledc_channel_config_t pwm_channel = {
        .gpio_num = PWM_PIN,
        .speed_mode = PWM_MODE,
        .channel = PWM_CHANNEL,
        .timer_sel = PWM_TIMER,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&pwm_channel);

    while (1)
    {
        for(int duty = 0; duty <= 255; duty += 25)
        {
            ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
            ledc_update_duty(PWM_MODE, PWM_CHANNEL);

            printf("Duty Cycle: %d\n", duty);

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}