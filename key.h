/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef _KEY_H
  #define _KEY_H

/***************************** Include files *******************************/
// FreeRTOS include files
  #include "FreeRTOS.h"
  #include "task.h"
  #include "semphr.h"
  #include "semaphoreHandlers.h"
  #include "queueHandlers.h"

/*****************************   Constants   *******************************/
/*****************************   Functions   *******************************/
BOOLEAN get_keyboard( INT8U* );
BOOLEAN peek_keyboard( INT8U* );
void key_Task(void*);

/****************************** End Of Module *******************************/
#endif
