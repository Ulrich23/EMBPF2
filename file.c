/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
//#include "glob_def.h"
//#include "rtcs.h"
#include "file.h"
//#include "tmodel.h"
#include "key.h"
#include "uartTask.h"
#include "lcd.h"

/*****************************    Defines    *******************************/
#define MAX_FILES  8

typedef struct
{
  BOOLEAN (*put)(INT8U);   // Pointer to device put function
  BOOLEAN (*get)(INT8U*);  // Pointer to device get function
} fcb;  //file control block

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
fcb pof[MAX_FILES];	            // Pool of files

/*****************************   Functions   *******************************/

BOOLEAN put_file( FILE file, INT8U ch )
{
  if( pof[(int)file].put )
    pof[(int)file].put( ch );
}

BOOLEAN get_file( FILE file, INT8U *pch )
{
  BOOLEAN result = FALSE;

  if( pof[(int)file].get )
    result = pof[(int)file].get( pch );
  return( result );
}


INT8U init_files() //extern
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8U i;

  for( i = 0; i < MAX_FILES; i++ )
  {
	  pof[i].put = NULL;
	  pof[i].get = NULL;
  }
  pof[COM1].put = uart0_put_q;
  pof[COM1].get = uart0_get_q; // pof = pool of files
  pof[COM2].put = wr_ch_LCD;
  pof[COM3].get = get_keyboard;
  pof[COM4].get = peek_keyboard;

  return( 1 );
}

/****************************** End Of Module *******************************/
