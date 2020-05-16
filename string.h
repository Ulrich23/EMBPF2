/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#include "file.h"
// FreeRTOS include files
#include "FreeRTOS.h"   // IMPORTANT!!! Always include FreeRTOS.h before task.h or queue.h etc.
#include "task.h"

void putc1( FILE, unsigned char );
const void gfprintf( FILE, const char*, ...);
const void gsprintf( INT8U*, const char*, ...);
const void gprintf( const char *, ...);

void clear_lcd_display();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
