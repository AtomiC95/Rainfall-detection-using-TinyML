#pragma once
#include <stdint.h>

#include <array>
#include <stdexcept>
#include <vector>
#include "Constants.h"

// #define pdSECOND pdMS_TO_TICKS(1000)

class DataProvider
{
public:
    uint32_t I2S_SAMPLE_RATE = 8000;


    uint8_t I2S_WS_I2S0 = 25;
    uint8_t I2S_SD_I2S0 = 33;
    uint8_t I2S_SCK_I2S0 = 26;

    // outbuffers are filled in one call of Read
    // a buffer array is written to according to the following pattern
    // [LSB, sample1-left, sample2-left, MSB, LSB, sample1-right, sample2-right, MSB]
    std::array<int16_t, bufferdepth> outbuffer_i2s0;
    int16_t i_data_storage = 0; // data storage index
    int samples_read = 0;
    size_t bytesIn_I2S0 = 0;    // tells when outbuffer is full

    std::vector<int16_t> data; // data for the feature provider
    DataProvider() = default;
    ~DataProvider() = default;

    bool Init();
    std::vector<float> Read();

private:
};
