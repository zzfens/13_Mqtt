#include "mqttto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_tls.h"
#include "cJSON.h"
#include "ledto.h"
#include "mqtt_client.h"
#define MQTT_SUBSCRIBE_TOPIC "test/topic"
#define MQTT_PUSH_TOPIC "test/topic/push"
#define MQTT_URL "mqtt://192.168.137.1:1883"
static const char *TAG = "MQTT_EXAMPLE";
static esp_mqtt_client_handle_t mqtt_client;
static int reconnect_count = 0;
static const int max_reconnect_count = 5;  // 最大重连次数
static const int reconnect_interval_ms = 5000;  // 重连间隔时间（毫秒）
cJSON *item = NULL;
msg_config_t msgn_config;
int led_state =0;
int oldled_state =0;


// MQTT 事件处理函数
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    if(client){
        switch ((esp_mqtt_event_id_t)event_id) {
            case MQTT_EVENT_CONNECTED:
                ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
                msg_id = esp_mqtt_client_subscribe(client, "test/topic", 0);
                reconnect_count = 0;  // 连接成功，重置重连计数
                break;
            case MQTT_EVENT_DISCONNECTED:
                ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
                if (reconnect_count < max_reconnect_count) {
                    ESP_LOGI(TAG, "Attempting to reconnect in %d ms (attempt %d/%d)", 
                             reconnect_interval_ms, reconnect_count + 1, max_reconnect_count);
                    reconnect_count++;
                    vTaskDelay(reconnect_interval_ms / portTICK_PERIOD_MS);
                    esp_mqtt_client_reconnect(client);
                } else {
                    ESP_LOGE(TAG, "Max reconnect attempts reached. Giving up.");
                }
                break;
            case MQTT_EVENT_SUBSCRIBED:
                ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
                break;
            case MQTT_EVENT_UNSUBSCRIBED:
                ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
                break;
            case MQTT_EVENT_PUBLISHED:
                ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
                break;
            case MQTT_EVENT_DATA:
                ESP_LOGI(TAG, "MQTT_EVENT_DATA");
                printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
                printf("TOPIC=%.*s\r\n", event->topic_len, event->data);

                cJSON *pJsonRoot = cJSON_Parse(event->data);
                if(pJsonRoot){
                    item = cJSON_GetObjectItem(pJsonRoot, "msg");
                    //如果是否json格式数据
                    led_state = atoi(cJSON_Print(item));
                    if(led_state == oldled_state){
                        esp_mqtt_module_publish(MQTT_PUSH_TOPIC,"0",1,0);
                    }
                    else{
                        led_turn(led_state);
                        esp_mqtt_module_publish(MQTT_PUSH_TOPIC,"1",1,0);
                        oldled_state = led_state;
                    }
                    

                }

    
    
                
                break;
            case MQTT_EVENT_ERROR:
                ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
                break;
            default:
                ESP_LOGI(TAG, "Other event id:%d", event->event_id);
                break;
        }

    }
    
}


// MQTT 初始化函数
void esp_mqtt_module_init() {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_URL,  // 设置 MQTT 代理的 URL
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

// MQTT 发布消息函数
void esp_mqtt_module_publish(const char *topic, const char *message, int qos, int retain) {
    if (mqtt_client) {
        int msg_id = esp_mqtt_client_publish(mqtt_client, topic, message, 0, qos, retain);
        ESP_LOGI(TAG, "sent publish message, msg_id=%d", msg_id);
    }
}

// MQTT 订阅主题函数
void esp_mqtt_module_subscribe(const char *topic, int qos) {
    if (mqtt_client) {
        int msg_id = esp_mqtt_client_subscribe(mqtt_client, topic, qos);
        ESP_LOGI(TAG, "sent subscribe topic, msg_id=%d", msg_id);
    }
}

// MQTT 取消订阅主题函数
void esp_mqtt_module_unsubscribe(const char *topic) {
    if (mqtt_client) {
        int msg_id = esp_mqtt_client_unsubscribe(mqtt_client, topic);
        ESP_LOGI(TAG, "sent unsubscribe topic, msg_id=%d", msg_id);
    }
}

// MQTT 停止函数
void esp_mqtt_module_stop() {
    if (mqtt_client) {
        esp_mqtt_client_stop(mqtt_client);
        esp_mqtt_client_destroy(mqtt_client);
        mqtt_client = NULL;
    }
}    