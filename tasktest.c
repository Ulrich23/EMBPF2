#include "tasktest.h"


void myTaskTest (void *p) // Blinks multicolour on-board LED Green and Red switching every second
{

    TickType_t myLastUnblock;
    myLastUnblock = xTaskGetTickCount();
    while(1) //Tasks must NEVER exit, so while loop must continue forever
    {
        display_color(GREEN);
        //vTaskDelay(pdMS_TO_TICKS ( 1000 ) ); // 1000 ms delay (not precise)
        vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 1000 ) ); // Accurate 1000ms delay
        display_color(RED);
        //vTaskDelay(pdMS_TO_TICKS ( 1000 ) );
        vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 1000 ) );

    }
}
