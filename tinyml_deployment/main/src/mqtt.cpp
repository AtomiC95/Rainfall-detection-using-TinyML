#include "mqtt.h"

#include <iostream>

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
  esp_mqtt_client_handle_t client = event->client;
  return ESP_OK;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                        int32_t event_id, void *event_data) {
  ESP_LOGD(Tagmqtt, "Event dispatched from event loop base=%s, event_id=%d",
           base, event_id);
  //mqtt_event_handler_cb((esp_mqtt_event_handle_t)event_data);
}

int mqtt_app_start(esp_mqtt_client_handle_t client) {
  int msg_id = esp_mqtt_client_subscribe(client, "/qos1",1);
  return msg_id;
}
