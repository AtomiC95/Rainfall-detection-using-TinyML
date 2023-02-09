#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "test_sdcard.h"
#include "main_functions.h"
#include <cstdio>
#include <string.h>

#include <chrono>
#include <iostream>

#include "DataProvider.h"
// #include "TrainDataRecorder.h"
// #include "main_functions.h"
#include "mqtt.h"
#include "wifi.h"

// using namespace std::chrono;

#define pdSECOND pdMS_TO_TICKS(1000)

// void tf_main(int argc, char* argv[]) {
//   setup();
//   while (true) {
//     loop();
//   }
// }

void collect_data()
{
  DataProvider data_provider;
  Database sdcard;
  data_provider.Init();
  sdcard.sdcard_init();
  
  esp_mqtt_client_config_t mqtt_cfg = {
      .uri = "mqtt://192.168.178.80",
  };
  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  nvs_flash_init();
  esp_netif_init();
  wifi_init_sta();
  // esp_event_loop_create_default();
  esp_mqtt_client_register_event(client,
                                 (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID,
                                 mqtt_event_handler, client);
  int counter = 0;
  int start_loop = 1;
  int start_loop_comp = 0;
  vTaskDelay(1 * pdSECOND);
  while (true)
  {
    // start_loop_comp = mqtt_app_start(client);
    if (true) // start_loop == start_loop_comp
    {
      if (counter == 16 * record_seconds) // samplerate / samples_gahtered = counter -> 8000/512 = 15,625 ~ 16
      {
        vTaskDelay(1 * pdSECOND);
        sdcard.unmount();
        printf("done!\n");
        while (true)
        {
          vTaskDelay(1 * pdSECOND);
        }
      }
      data_provider.Read();
      sdcard.saveData(data_provider.outbuffer_i2s0, data_provider.samples_read);
      counter++;
    }
  }
}
extern "C" void app_main()
{
  // xTaskCreate((TaskFunction_t)&tf_main, "tensorflow", 10 * 1024, NULL, 8, NULL);
  // vTaskDelete(NULL);

  xTaskCreate((TaskFunction_t)&collect_data, "collect_data", 25 * 1024, NULL, 8, NULL);
  vTaskDelete(NULL);
}