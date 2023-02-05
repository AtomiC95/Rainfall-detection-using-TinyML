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

// void DataProvider::FlushData()
// {
//   for (int16_t t = 0; t < data_storage_len; t++)
//   {
//     if (t % delay_rate == 0)
//     {
//       vTaskDelay(1);
//       // std::cout << "hello2\n";
//     }
//     std::cout << flush_data[t] << "\n";
//   }
//   this->i_data_storage = 0;
//   // vTaskDelay(1 *pdSECOND);
// }

// void DataProvider::HandleFullOutBuffer()
// {
//   for (uint8_t i = 2; i < this->bytesPerChannel - 4; i++)
//   {
//     // this->mean_left_i2s0 += this->;

//     this->flush_data[i_data_storage] = outbuffer_i2s0[i];
//     this->i_data_storage++;

//     // this->mean_left_i2s1 += this->outbuffer_i2s1[i];
//   }
  // for (uint8_t i = this->bytesPerChannel + 2; i < bufferdepth; i++) {
  //   this->mean_right_i2s0 += this->outbuffer_i2s0[i];
  //   this->mean_right_i2s1 += this->outbuffer_i2s1[i];
  // }
  // for (uint8_t i = 9; i < bytesIn_I2S0; i++) {
  //   this->mean_right_i2s0 += this->outbuffer_i2s0[i];
  // }
  // for (uint8_t i = 1; i < 8; i++) {
  //   this->mean_left_i2s0 += this->outbuffer_i2s0[i];
  // }
  // this->mean_left_i2s0 /= 1; // bytesIn_I2S0;

  // this->flush_data[i_data_storage] = mean_left_i2s0;
  // this->i_data_storage++;
// }

// void DataProvider::PrintOutBuffer()
// {
//   std::cout << bytesIn_I2S0 << "\n";
//   std::cout << "I2S0: ";
//   for (int i = 0; i < bufferdepth; i++)
//   {
//     std::cout << this->outbuffer_i2s0[i] << " ";
//   }
//   std::cout << "\n";
// }

std::vector<float> DataProvider::Read()
{
  // std::cout << "Test 1" << std::endl;
  //this->mean_left_i2s0 = 0;

  // i2s_read runs until outbuffer is full
  esp_err_t result_I2S0 =
      i2s_read(I2S_PORT_ONE, &outbuffer_i2s0, sizeof(int16_t) * bufferdepth,
               &bytesIn_I2S0, 1000); // 1000: RTOS ticks to wait for values in
                                     // outbuffer until throwing an error

  // esp_err_t result_I2S1 =
  //     i2s_read(I2S_PORT_TWO, &outbuffer_i2s1, sizeof(int16_t) * bufferdepth,
  //              &bytesIn_I2S1, 1000);

  if (!(result_I2S0 == ESP_OK))
  {
    std::cout << "I2S read error" << std::endl;
    return {0.0, 0.0, 0.0, 0.0};
  }
  samples_read = bytesIn_I2S0 /sizeof(int16_t);

  // for (int i = 0; i < samples_read; i++)
  // {
  //   printf("%d\n", outbuffer_i2s0[i]);
  // }
  vTaskDelay(1);

  // if (DEBUG)
  // {
  //   this->PrintOutBuffer();
  // }

  // if (i_data_storage % delay_rate == 0)
  // {
  //   // std::cout << i_data_storage << std::endl;
  //   vTaskDelay(1);
  //   // std::cout << "hello1\n";
  // }

  // if (bytesIn_I2S0 == sizeof(int16_t) * bufferdepth) //
  // {
  //   this->HandleFullOutBuffer();
  // }

  // if (this->i_data_storage == data_storage_len)
  // {
  //   // std::cout << "Test 2" << std::endl;
  //   //  auto t1 = high_resolution_clock::now();
  //   this->FlushData();
  //   // auto t2 = high_resolution_clock::now();
  //   // duration<double, std::milli> ms_double = t2 - t1;
  //   // std::cout << ms_double.count() << "ms\n";
  // }

  // std::cout << "bytesIn_I2S0: " << bytesIn_I2S0 << "i_data_storage: "
  //           << i_data_storage << std::endl;
  return {0.0, 0.0, 0.0, 0.0};
}

void DataProvider::ReadBlocking()
{
  for (int16_t i = 0; i < data_storage_len - 1; i++)
  {
    this->Read();
  }
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

  // result = i2s_driver_install(I2S_PORT_TWO, &i2s_config, 0, NULL);

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

  // i2s_pin_config_t pin_config_TWO = {};
  // pin_config_TWO.bck_io_num = I2S_SCK_I2S1;
  // pin_config_TWO.ws_io_num = I2S_WS_I2S1;
  // pin_config_TWO.data_out_num = -1;
  // pin_config_TWO.data_in_num = I2S_SD_I2S1;

  // result = i2s_set_pin(I2S_PORT_TWO, &pin_config_TWO);
  // if (result != ESP_OK) {
  //   std::cout << "i2s_set_pin(I2S_PORT_TWO, &pin_config_TWO) -> " << result
  //             << std::endl;
  // }
  return true;
}