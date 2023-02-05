#pragma once

#define pdSECOND pdMS_TO_TICKS(1000)
#define bufferdepth 512 // defines len of outbuffer
#define PIN_MAP 1337
#define record_seconds 10
#define final_sampling_rate 2000 // maybe 4k
#define data_storage_len (int)(record_seconds * final_sampling_rate)
#define delay_rate 2000 // number of loops on which to vtaskdelay so RTOS can call IDLE_Task
#define total_num_channels 1