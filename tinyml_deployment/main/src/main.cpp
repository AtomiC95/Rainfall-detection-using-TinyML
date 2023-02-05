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
// #include "mqtt.h"
// #include "wifi.h"

//using namespace std::chrono;

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
  // sdcard.saveData();
  int counter = 0;
  vTaskDelay(1 * pdSECOND);
  while (true)
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
    sdcard.saveData(data_provider.outbuffer_i2s0,data_provider.samples_read);
    counter++;
  }
}
extern "C" void app_main()
{
  // xTaskCreate((TaskFunction_t)&tf_main, "tensorflow", 10 * 1024, NULL, 8, NULL);
  // vTaskDelete(NULL);

  xTaskCreate((TaskFunction_t)&collect_data, "collect_data", 25 * 1024, NULL, 8, NULL);
  vTaskDelete(NULL);
}