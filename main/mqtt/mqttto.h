#ifndef _MQTTTO_H_
#define _MQTTTO_H_

typedef struct{
	char *msg;

} msg_config_t;

// MQTT 初始化函数
void esp_mqtt_module_init();

// MQTT 发布消息函数
void esp_mqtt_module_publish(const char *topic, const char *message, int qos, int retain);

// MQTT 订阅主题函数
void esp_mqtt_module_subscribe(const char *topic, int qos);

// MQTT 取消订阅主题函数
void esp_mqtt_module_unsubscribe(const char *topic);

// MQTT 停止函数
void esp_mqtt_module_stop();

#endif // ESP_MQTT_MODULE_H    

