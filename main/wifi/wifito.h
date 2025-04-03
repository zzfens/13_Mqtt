#ifndef _WIFITO_H_
#define _WIFITO_H_


#include <string.h>
#include "esp_event.h"
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void wifi_init_sta(void);


#endif