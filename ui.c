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
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
//#include "systick.h"
//#include "tmodel.h"
//#include "systick.h"
#include "ui.h"
//#include "rtc.h"
#include "string.h"
//#include "glob_def.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U counter_star;
INT8U counter_square;
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
/****************************** End Of Module *******************************/
