/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/
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
