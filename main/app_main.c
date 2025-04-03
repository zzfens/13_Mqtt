

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqttto.h"
#include "wifito.c"
#define CONFIG_WIFI_SSID  "没命的热"
#define CONFIG_WIFI_PASSWORD "zz123456"
#include "ledto.h"


void app_main(void)
{
    /* 初始化NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    wifi_init_sta();
    led_init();
    esp_mqtt_module_init();

    

}
