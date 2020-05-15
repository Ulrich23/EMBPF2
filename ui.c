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

void ui_Task(void* p)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/

{

    INT8U i;
    INT8U  InBuf[128];
    INT8U ch;

    while (1)
    {

        if (get_file(COM1, &ch))                                        // get char from uart (COM1)
        {
            if (i < 128)
            {
                InBuf[i++] = ch;                                              // puts char in the buffer InBuf
                put_file(COM1, ch);                                           // puts the char back to uart so we can see what we type in the terminal
                if (ch == '\r')                                               // if the current char is 'Enter' then...
                {
                    if (InBuf[0] == '1' && i >= 7)
                    {
						
                        INT8U hour = (InBuf[1] - '0') * 10 + InBuf[2] - '0';
                        INT8U min = (InBuf[3] - '0') * 10 + InBuf[4] - '0';
                        INT8U sec = (InBuf[5] - '0') * 10 + InBuf[6] - '0';
                        if ( (hour > 23) && (min > 59) && (sec > 59) ); // Check if the clock values are valid
                        {
                            gfprintf(COM1, "Please enter a valid time hour/minute/sec aka (235959)");
                                
                        }
                        struct time_day day;
                        //gfprintf( COM2, "%c%cREMOVE NOZZLE   ", 0x1B, 0x80);
						//gfprintf( COM2, "%c%cFuel ThatBitch %01d", 0x1B, 0xC0, get_button_id());
                        
                    }
                   
                    
                }
            i = 0;
            }

        }
    }
    
    

}

//if(( InBuf[0] == '1' ) && ( i >= 7 ))
  //        
    //          set_hour( (InBuf[1]-'0')*10+InBuf[2]-'0');
      //        set_min( (InBuf[3]-'0')*10+InBuf[4]-'0');
        //      set_sec( (InBuf[5]-'0')*10+InBuf[6]-'0');

/****************************** End Of Module *******************************/
