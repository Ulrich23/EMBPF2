/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

/***************************** Include files *******************************/
#include "clockTask.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/
void clock_Task (void *p)
/*****************************************************************************
*   Input    : NULL
*   Output   : -
*   Function : Updates today time in seconds minutes
******************************************************************************/
{
    struct time_day today;
    today.seconds = 0;
    today.minutes = 0;
    today.hours   = 0;

    TickType_t myLastUnblock;
    myLastUnblock = xTaskGetTickCount();


    while(1) //Tasks must NEVER exit, so while loop must continue forever
    {
        //vTaskDelay(pdMS_TO_TICKS ( 1000 ) ); // 1000 ms delay (not precise)
        vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 1000 ) ); // Accurate 1000ms delay

        if(today.seconds == 59)
        {
          today.seconds = 0;

          if(today.minutes == 59)
          {
            today.minutes = 0;
            if(today.hours == 24)
            {
              today.hours = 0;
            }
            else
            {
              today.hours ++;
            }
          }
          else
          {
            today.minutes++;
          }
        }
        else
        {
          today.seconds++;
        }


        // Update the clock that the other tasks has access to
        xQueueOverwrite(Q_CLOCK, &today);

    }
}


/****************************** End Of Module *******************************/
