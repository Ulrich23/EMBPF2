/*
 * string.h
 *
 *  Created on: 26/12/2011
 *      Author: Morten
 */
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
