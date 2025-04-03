#include "wifito.h"
#include <string.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#define SSID  "没命的热"
#define PASSWORD "zz123456"
static const char *TAG = "wifi station";
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    //当WiFi STA启动时，调用esp_wifi_connect()尝试连接
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
        esp_wifi_connect();
    //当WiFi STA断开连接时，重新连接
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
        esp_wifi_connect();
    //当STA获取到IP地址时，记录获取到的IP地址
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

void wifi_init_sta(void)
{
    //初始化网络接口
    ESP_ERROR_CHECK(esp_netif_init());
    //创建默认事件循环
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    //创建默认的Wi-Fi STA接口
    esp_netif_create_default_wifi_sta();
	//初始化Wi-Fi驱动程序
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    //注册Wi-Fi和IP事件处理函数
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    //注册WiFi和IP事件处理函数
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));
	//配置Wi-Fi连接参数（SSID和密码）
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = SSID,
            .password = PASSWORD,
        },
    };
    //设置Wi-Fi模式为STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    //启动Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());
}

