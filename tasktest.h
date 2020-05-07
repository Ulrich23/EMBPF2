#ifndef TASKTEST_H_
#define TASKTEST_H_

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

// FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"

// Include Task Handlers for each task for task communication
#include "taskHandlers.h"

// Display Color (For debugging purposes)
#include "display_color.h"


void myTaskTest (void *p);


#endif /* TASKTEST_H_ */
