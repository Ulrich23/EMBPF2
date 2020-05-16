/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef _LCD_H
  #define _LCD_H

/***************************** Include files *******************************/
// FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "queueHandlers.h"
/*****************************    Defines    *******************************/
// Special ASCII characters
// ------------------------

#define LF		0x0A
#define FF		0x0C
#define CR		0x0D

#define ESC		0x1B


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void wr_str_LCD( INT8U* );
void move_LCD( INT8U, INT8U );
INT8U wr_ch_LCD( INT8U Ch );

void lcd_Task(void*);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/
#endif
