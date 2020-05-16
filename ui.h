/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef _UI_H
  #define _UI_H

/***************************** Include files *******************************/
#include <stdint.h>
#include<stdarg.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
#include "string.h"
#include "file.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Include Task Handlers for each task for task communication
#include "taskHandlers.h"
#include "queueHandlers.h"

#include "storage.h"


/*****************************    Defines    *******************************/

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

void ui_Task(void* p);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/
void ui_key_task(void* p);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Not used
******************************************************************************/
BOOLEAN get_star_key();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Testing if the star key is pressed, removing it from Q_KEY after
******************************************************************************/
BOOLEAN get_square_key();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Testing if the square key is pressed, removing it from Q_KEY after 
******************************************************************************/
/****************************** End Of Module *******************************/
#endif
