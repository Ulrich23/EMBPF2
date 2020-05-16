/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef FILES_CLOCKTASK_H_
#define FILES_CLOCKTASK_H_

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

// FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"


// Include Task Handlers for each task for task communication
#include "taskHandlers.h"
#include "queueHandlers.h"
#include "semaphoreHandlers.h"

// Include storage
#include "storage.h"


// Display Color (For debugging purposes)
//#include "display_color.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void clock_Task (void *p);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/


/****************************** End Of Module *******************************/


#endif /* FILES_CLOCKTASK_H_ */
