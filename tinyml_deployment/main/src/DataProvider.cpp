#include "DataProvider.h"

#include <iostream>

#include "driver/i2s.h"
#include <chrono>

// I2S_PORT_ONE: maps to a range of pins, choice is arbitrary
#define I2S_PORT_ONE I2S_NUM_0
#define I2S_PORT_TWO I2S_NUM_1
#define DEBUG false

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

std::vector<float> DataProvider::Read()
{
  // std::cout << "Test 1" << std::endl;

  // i2s_read runs until outbuffer is full
  esp_err_t result_I2S0 =
      i2s_read(I2S_PORT_ONE, &outbuffer_i2s0, sizeof(int16_t) * bufferdepth,
               &bytesIn_I2S0, 1000); // 1000: RTOS ticks to wait for values in
                                     // outbuffer until throwing an error
  if (!(result_I2S0 == ESP_OK))
  {
    std::cout << "I2S read error" << std::endl;
    return {0.0, 0.0, 0.0, 0.0};
  }
  samples_read = bytesIn_I2S0/ sizeof(int16_t);

  vTaskDelay(1);

  return {0.0, 0.0, 0.0, 0.0};
}

bool DataProvider::Init()
{
  i2s_driver_config_t i2s_config = {};
  i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
  i2s_config.sample_rate = I2S_SAMPLE_RATE;
  i2s_config.bits_per_sample = (i2s_bits_per_sample_t)16;
  i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;
  i2s_config.communication_format =
      i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S |
                        I2S_COMM_FORMAT_I2S_MSB); //  | I2S_COMM_FORMAT_I2S_MSB
  i2s_config.intr_alloc_flags = 0;
  i2s_config.dma_buf_count = 4;
  i2s_config.dma_buf_len = 1024;
  i2s_config.use_apll = false;
  i2s_config.tx_desc_auto_clear = false;
  i2s_config.fixed_mclk = 0;

  auto result = i2s_driver_install(I2S_PORT_ONE, &i2s_config, 0, NULL);

  // Set I2S pin configuration
  // need to set a flag
  i2s_pin_config_t pin_config_one = {};
  pin_config_one.bck_io_num = I2S_SCK_I2S0;
  pin_config_one.ws_io_num = I2S_WS_I2S0;
  pin_config_one.data_out_num = -1;
  pin_config_one.data_in_num = I2S_SD_I2S0;

  // i2s_driver_uninstall();
  result = i2s_set_pin(I2S_PORT_ONE, &pin_config_one);
  if (result != ESP_OK)
  {
    std::cout << "result = i2s_set_pin(I2S_PORT_ONE, &pin_config_one) -> "
              << result << std::endl;
  }

  return true;
}