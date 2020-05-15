/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: ui.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include "ui.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U counter_star;
INT8U counter_square;
INT8U i;
INT8U  InBuf[128];

/*****************************   Functions   *******************************/

BOOLEAN get_star_key()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  BOOLEAN result_star = FALSE;
  INT8U ch;
  if( get_file( COM4, &ch )) // Peek in file
  {
    if( ch == '*')
    {
      INT8U int_ch = 0;
      if( get_file( COM3, &int_ch ))
      {
        result_star = TRUE;
        counter_star = counter_star + 1;
      }
    }
  }
  return(result_star);
}

BOOLEAN get_square_key()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    BOOLEAN result_square = FALSE;
    INT8U ch;
    if( get_file( COM4, &ch )) // Peek in file
    {
      if( ch == '#')
      {
        INT8U int_ch = 0;
        if( get_file( COM3, &int_ch ))
        {
          result_square = TRUE;
          counter_square = counter_square + 1;
        }
      }
    }
    return(result_square);
}

//void ui_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
/*
INT8U ch;

if( get_file( COM1, &ch ))                                        // get char from uart (COM1)
{
    if( i < 128 )
    {
        InBuf[i++] = ch;                                              // puts char in the buffer InBuf
        put_file( COM1, ch );                                           // puts the char back to uart so we can see what we type in the terminal
        if( ch == '\r' )                                                // if the current char is 'Enter' then...
        {
            if()
            {
                
            }
        }
        i = 0;
    }
}


*/
/****************************** End Of Module *******************************/
