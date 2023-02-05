#pragma once
#include <stdint.h>

#include <array>
#include <stdexcept>
#include <vector>
#include "Constants.h"

// #define pdSECOND pdMS_TO_TICKS(1000)
// #define bufferdepth 512 // defines len of outbuffer
// #define PIN_MAP 1337
// #define record_seconds 1
// #define final_sampling_rate 2000 // maybe 4k
// #define data_storage_len (int)(record_seconds * final_sampling_rate)
// #define delay_rate 2000 // number of loops on which to vtaskdelay so RTOS can call IDLE_Task
// #define total_num_channels 1
// 1337 -> niko
// 0 -> rafael

class DataProvider
{
public:
    uint32_t I2S_SAMPLE_RATE = 8000;


    uint8_t I2S_WS_I2S0 = 25;
    uint8_t I2S_SD_I2S0 = 33;
    uint8_t I2S_SCK_I2S0 = 26;
    // uint8_t I2S_WS_I2S1 = 27;
    // uint8_t I2S_SD_I2S1 = 33;
    // uint8_t I2S_SCK_I2S1 = 14;

    //int16_t mean_left_i2s0 = 0;
    //int16_t mean_right_i2s0 = 0;
    //int16_t mean_left_i2s1 = 0;
    //int16_t mean_right_i2s1 = 0;
    // outbuffers are filled in one call of Read
    // a buffer array is written to according to the following pattern
    // [LSB, sample1-left, sample2-left, MSB, LSB, sample1-right, sample2-right, MSB]
    std::array<int16_t, bufferdepth> outbuffer_i2s0;
    //std::array<int16_t, bufferdepth> outbuffer_i2s1;
    // flush_data aggregates the outbuffers for flushing to UART
    // it contains postprocessing on the data and discards msb and lsb
    //std::array<int16_t, data_storage_len> flush_data;
    int16_t i_data_storage = 0; // data storage index
    int samples_read = 0;
    size_t bytesIn_I2S0 = 0;    // tells when outbuffer is full
    //uint8_t bytesPerChannel = bufferdepth / 2;
    //uint8_t bytesPerChannelNoSB = bytesPerChannel - 2; // -2 because of msb and lsb

    std::vector<int16_t> data; // data for the feature provider
    DataProvider() = default;
    ~DataProvider() = default;

    // void PrintOutBuffer();
    //void FlushData();
    //void HandleFullOutBuffer();
    //void PrintOutBuffer();
    bool Init();
    void ReadBlocking();
    std::vector<float> Read();

private:
};
