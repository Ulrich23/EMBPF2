#ifndef FILES_TASKHANDLERS_H_
#define FILES_TASKHANDLERS_H_

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t spiTaskHandle;     // Task Handle for spiTask
TaskHandle_t myTaskTestHandle;  // Task handle for myTaskTest

#endif /* FILES_TASKHANDLERS_H_ */
