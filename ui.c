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
    INT16U myarr[2] = {0};
    TickType_t myLastUnblock;
    myLastUnblock = xTaskGetTickCount();

	

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
					if (InBuf[0] == '1')
                    {
						gfprintf(COM1, "\n To set the time press 2 followed by hhmmss, then enter\n\r");
                        char a= 0;
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS ( 100 ) );
						gfprintf(COM1, "%cTime Example: 2125630 is 12:56:30 \n\r", a);
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, "\n To set fuel prices press 3 followed by:\n\r");
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, " 1 For LeadFree92   followed by the price\n\r");
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, " 2 For LeadFree95   followed by the price\n\r");
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, " 3 For Diesel       followed by the price\n\r");
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, "Gas Example: 320982 is 09.82 DKK \n\r");
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, "\n Get a Query report of total sales of gasoline sorted by product,\n\r");
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, "\n the sum of all cash purchases, sum of all card purchases, and\n\r");
                        vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(100));
						gfprintf(COM1, "\n the total operating time of the pump, by writing 4 followed by enter\n");
					}
     //               else if (InBuf[0] == '2' && i >= 7)
     //               {

     //                   INT8U hour = (InBuf[1] - '0') * 10 + InBuf[2] - '0';
     //                   INT8U min = (InBuf[3] - '0') * 10 + InBuf[4] - '0';
     //                   INT8U sec = (InBuf[5] - '0') * 10 + InBuf[6] - '0';
     //                   if ((hour > 23) || (min > 59) || (sec > 59)) // Check if the clock values are valid
     //                   {
     //                       gfprintf(COM1, "\nPlease enter a valid time hour minute sec aka 235959\n");
     //                   }
     //              
     //                   else
     //                   {
     //                       
     //                       struct time_day day;
     //                       day.hours = hour;
     //                       day.seconds = sec;
     //                       day.minutes = min;
     //                       xQueueOverwrite(Q_CLOCK, &day);
     //                       xQueuePeek(Q_CLOCK, &day, 0);
     //                       gfprintf(COM1, "\nTime of day set to %02d:%02d:%02d\n", day.hours, day.minutes, day.seconds);
     //                   }
     //                   
     //                   //gfprintf( COM2, "%c%cREMOVE NOZZLE   ", 0x1B, 0x80);
					//	//gfprintf( COM2, "%c%cFuel ThatBitch %01d", 0x1B, 0xC0, get_button_id());
     //                   
     //               }
     //               else if(InBuf[0] == '3' && i >= 5)
     //               {
     //                   struct gas_price currentPrice;
     //                   if (InBuf[1] == '1')
     //                   {
     //                       FP32 LF92price = ((InBuf[2] - '0') * 1000 + (InBuf[3] - '0') * 100 + (InBuf[4] + '0') * 10 + InBuf[5]) / 100;
     //                       currentPrice.LF92_Price = LF92price;
     //                       xQueueOverwrite(Q_GASPRICES, &currentPrice);
     //                       bcd(currentPrice.LF92_Price, myarr);
     //                       gfprintf(COM1, "\n Lead free 92 price set to %02d.%02d\n",  myarr[0], myarr[1]);
     //                   }
     //                   else if(InBuf[1] == '2')
     //                   {
     //                       FP32 LF95price = ((InBuf[2] - '0') * 1000 + (InBuf[3] - '0') * 100 + (InBuf[4] + '0') * 10 + InBuf[5]) / 100;
     //                       currentPrice.LF95_Price = LF95price;
     //                       xQueueOverwrite(Q_GASPRICES, &currentPrice);
     //                       bcd(currentPrice.LF95_Price, myarr);
     //                       gfprintf(COM1, "\n Lead free 95 price set to %02d.%02d\n",  myarr[0], myarr[1]);
					//	}
     //                   else if(InBuf[1] == '3')
     //                   {
     //                       FP32 DIESEL = ((InBuf[2] - '0') * 1000 + (InBuf[3] - '0') * 100 + (InBuf[4] + '0') * 10 + InBuf[5]) / 100;
     //                       currentPrice.DIESEL_Price = DIESEL;
     //                       xQueueOverwrite(Q_GASPRICES, &currentPrice);
     //                       bcd(currentPrice.DIESEL_Price, myarr);
     //                       gfprintf(COM1, "\n Diesel price set to %02d.%02d\n",  myarr[0], myarr[1]);
					//	}

					//}
     //               else if (InBuf[0] == '4' && i == 1)
     //               {
     //                   FP32 total_sales = 0;
     //                   for (int i = 0; i < purchase_counter; ++i)
     //                   {
     //                       FP32 gasolina = 0;
     //                       struct gas_price currentPrice;
     //                       xQueuePeek(Q_GASPRICES, &currentPrice, 0);
     //                       if (purchase_log[i].product == S_LF92)
     //                       {
     //                           gasolina = currentPrice.LF92_Price;
     //                       }
     //                       else if (purchase_log[i].product == S_LF95)
     //                       {
     //                           gasolina = currentPrice.LF95_Price;
     //                       }
     //                       else if (purchase_log[i].product == S_DIESEL)
     //                       {
     //                           gasolina = currentPrice.DIESEL_Price;
     //                       }
     //                       else
     //                       {
     //                           gasolina = 1000000; //bare cash
     //                       }
     //                       total_sales += purchase_log[i].quantity * gasolina;
     //                       bcd(total_sales, myarr);

     //                   }
     //                   gfprintf(COM1, "\n WE MADE BARE CASH BOOII %02d.%02d\n", myarr[0], myarr[1]);

     //               }
                   
                 i = 0;
                }

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
