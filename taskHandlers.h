#ifndef FILES_TASKHANDLERS_H_
#define FILES_TASKHANDLERS_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t myTaskTestHandle;  // Task handle for myTaskTest
TaskHandle_t lcdTaskHandle;     // Task handle for lcdTaskHandle
TaskHandle_t keyTaskHandle;     // Task handle for keyTaskHandle
TaskHandle_t clockTaskHandle;    // Task handle for cockTaskHandle
TaskHandle_t displayMenuTaskHandle; // Task handle for displayMenuTaskHandle
#endif /* FILES_TASKHANDLERS_H_ */
