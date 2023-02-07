#pragma once
#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "/home/nikolas/esp/esp-idf/components/nvs_flash/include/nvs_flash.h"

static const char *TAGwifi = "wifi station";

void event_handler();  // changed from stati void to void
void wifi_init_sta();
