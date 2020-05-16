/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef _UART_H
  #define _UART_H

/***************************** Include files *******************************/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

// FreeRTOS include files
#include "FreeRTOS.h"   // IMPORTANT!!! Always include FreeRTOS.h before task.h or queue.h etc.
#include "task.h"
#include "queue.h"
#include "semphr.h"


// Display Color (For Debugging Purposes)
#include "display_color.h"

// Others
#include "taskHandlers.h"
#include "queueHandlers.h"
#include "semaphoreHandlers.h"
#include "storage.h"
#include "file.h"
#include "string.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
BOOLEAN uart0_put_q( INT8U );
BOOLEAN uart0_get_q( INT8U* );

void uart_tx_Task(void* p);
void uart_rx_Task(void* p);


extern void uart0_init( INT32U, INT8U, INT8U, INT8U );
/*****************************************************************************
*   Input    : -       baud, size, stopbits, paritybit
*   Output   : -
*   Function : Initialize uart 0
******************************************************************************/


/****************************** End Of Module *******************************/
#endif

