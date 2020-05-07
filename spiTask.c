#include "spiTask.h"


void spiTask (void *p)
{
    TickType_t myLastUnblock;
    myLastUnblock = xTaskGetTickCount();

    INT16U spiSendData = 0;
    while(1)
    {
        vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 1 ) ); // Block this task at all time
                                                                 // Wake up once every 1ms
                                                                 // Execute code.

        write_spi(spiSendData++);
    }
}
