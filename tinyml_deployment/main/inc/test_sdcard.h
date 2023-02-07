#pragma once
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <stdio.h>
#include <iostream>
#include <array>
#include "Constants.h"

static const char *TAG = "Test";
#define MOUNT_POINT "/sdcard"


class Database
{
public:
    struct stat st;

    int file_created = 0;
    sdmmc_card_t *card;
    esp_err_t ret;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    const char *INMP441 = MOUNT_POINT "/INMP441.txt";
    //const char mount_point[] = MOUNT_POINT;

    void unmount();
    void sdcard_init();
    void saveData(std::array<int16_t, bufferdepth> outbuffer_i2s0, int samples_read);

private:
};