#include "encoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define ENCODER_GPIO_A 33
#define ENCODER_GPIO_B 25

#define PCNT_HIGH_LIMIT 32767
#define PCNT_LOW_LIMIT  -32768

#define PULSOS_POR_REV 20
#define SAMPLE_TIME_MS 1000

static const char *TAG = "ENCODER";

static pcnt_unit_handle_t pcnt_unit = NULL;
static int last_count = 0;

void encoder_init(void)
{

    pcnt_unit_config_t unit_config = {
        .high_limit = PCNT_HIGH_LIMIT,
        .low_limit = PCNT_LOW_LIMIT,
    };

    pcnt_new_unit(&unit_config, &pcnt_unit);

    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };

    pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config);

    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_GPIO_A,
        .level_gpio_num = ENCODER_GPIO_B,
    };

    pcnt_channel_handle_t chan_a = NULL;
    pcnt_new_channel(pcnt_unit, &chan_a_config, &chan_a);

    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = ENCODER_GPIO_B,
        .level_gpio_num = ENCODER_GPIO_A,
    };

    pcnt_channel_handle_t chan_b = NULL;
    pcnt_new_channel(pcnt_unit, &chan_b_config, &chan_b);

    pcnt_channel_set_edge_action(
        chan_a,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE);

    pcnt_channel_set_level_action(
        chan_a,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    pcnt_channel_set_edge_action(
        chan_b,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE);

    pcnt_channel_set_level_action(
        chan_b,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    pcnt_unit_enable(pcnt_unit);
    pcnt_unit_clear_count(pcnt_unit);
    pcnt_unit_start(pcnt_unit);

    ESP_LOGI(TAG, "Encoder inicializado");
}

int encoder_get_count(void)
{
    int count = 0;
    pcnt_unit_get_count(pcnt_unit, &count);
    return count;
}

float encoder_get_rpm(void)
{
    int current_count = encoder_get_count();

    int delta = current_count - last_count;

    last_count = current_count;

    float revs = (float)delta / (PULSOS_POR_REV * 4);

    float rpm = revs * (60000.0 / SAMPLE_TIME_MS);

    return rpm;
}