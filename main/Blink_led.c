#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG 
#include "esp_log.h"
#include "esp_err.h"

#define LED 2
static uint8_t led_level = 0;
static const char *TAG = "Giezi_Project"; 

esp_err_t init_led(void){
    gpio_reset_pin(LED);
    esp_err_t err = gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Hardware: LED inicializado correctamente.");
    } else {
        ESP_LOGE(TAG, "Hardware: Error al configurar el pin %d", LED);
    }
    return err;
}

esp_err_t blink_led(void){
    led_level = !led_level;
    
    // Log de nivel Debug 
    ESP_LOGD(TAG, "Cambiando nivel logico a: %d", led_level);
    
    esp_err_t err = gpio_set_level(LED, led_level);
    
    if (err == ESP_OK) {
        // Log de nivel Info
        ESP_LOGI(TAG, "El LED ahora esta %s", led_level ? "ENCENDIDO" : "APAGADO");
    } else {
        // Log de nivel Warning
        ESP_LOGW(TAG, "No se pudo actualizar el estado fisico del LED");
    }
    
    return err;
}

void app_main(void){
    
    
    esp_log_level_set(TAG, ESP_LOG_INFO); 

    ESP_ERROR_CHECK(init_led());
    
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
       
        ESP_ERROR_CHECK(blink_led());

    }
}