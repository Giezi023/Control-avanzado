#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "encoder.h"

static const char *TAG = "MAIN";

void app_main(void)
{

    encoder_init();

    while (1)
    {

        int pulsos = encoder_get_count();

        float rpm = encoder_get_rpm();

        if (rpm > 0)
        {
            ESP_LOGI(TAG, "Pulsos: %d | RPM: %.2f | Sentido: Horario", pulsos, rpm);
        }
        else if (rpm < 0)
        {
            ESP_LOGI(TAG, "Pulsos: %d | RPM: %.2f | Sentido: Antihorario", pulsos, rpm);
        }
        else
        {
            ESP_LOGI(TAG, "Pulsos: %d | RPM: %.2f | Motor detenido", pulsos, rpm);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}