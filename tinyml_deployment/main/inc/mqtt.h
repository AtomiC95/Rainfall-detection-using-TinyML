#pragma once
#include <stdint.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "mqtt_client.h"
#include "/home/nikolas/esp/esp-idf/components/nvs_flash/include/nvs_flash.h"
#include <string> 

#define pdSECOND pdMS_TO_TICKS(1000)

static const char *Tagmqtt = "MQTT_TCP";

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);

void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data);

int mqtt_app_start(esp_mqtt_client_handle_t client);
