/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

/***************************** Include files *******************************/

#include "ui.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U counter_star;
INT8U counter_square;


/*****************************   Functions   *******************************/
//BOOLEAN sendString(char *myString, char *myVar)
//{
//    for (int i = 0; i < strlen(myString); ++i)
//    {
//        if(myString[i], )
//        put_file(COM1, myString[i]);
//        vTaskDelay(10);
//
//    }
//        
//}
void iota(INT32U num, char* myArray, INT8U arraySize)
{

    INT32U div = 1;
    for (INT8U i = 1; i < arraySize; ++i)
    {
        div *= 10;
    }

    INT32U val = 0;
    for (INT8U i = 0; i < arraySize; ++i)
    {
        val = (num % (div * 10)) / div;
        myArray[i] = val + '0';
        div /= 10;
    }

}
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

                        gfprintf(COM1, "\nTo set the time press 2 followed by hhmmss, then press enter\n\r");
						gfprintf(COM1, "Time Example: 2125630 is 12:56:30 \n\r");
                        gfprintf(COM1, "\n To set fuel prices press 3 followed by:\n\r");
                        gfprintf(COM1, " 1 For LeadFree92   followed by the price\n\r");
						gfprintf(COM1, " 2 For LeadFree95   followed by the price\n\r");
						gfprintf(COM1, " 3 For Diesel       followed by the price\n\r");
						gfprintf(COM1, "Then press enter\n\r");
						gfprintf(COM1, "Gas Example: 320982 sets the price of LeadFree95 to 09.82 DKK \n\r");

						gfprintf(COM1, "\nGet a Query report of total sales of gasoline sorted by product,\n\r");
						gfprintf(COM1, "the sum of all cash purchases, sum of all card purchases, and\n\r");
						gfprintf(COM1, "the total operating time of the pump, by writing 4 then press enter\n\r");
					}
                    else if (InBuf[0] == '2' && i >= 7) // Set time of day
                    {

                        INT8U hour = (InBuf[1] - '0') * 10 + InBuf[2] - '0';
                        INT8U min = (InBuf[3] - '0') * 10 + InBuf[4] - '0';
                        INT8U sec = (InBuf[5] - '0') * 10 + InBuf[6] - '0';
                        if ((hour > 23) || (min > 59) || (sec > 59)) // Check if the clock values are valid
                        {
                            gfprintf(COM1, "\nPlease enter a valid time hour minute sec aka 235959\n\r");
                        }
                   
                        else
                        {
                            struct time_day day;
                            day.hours = hour;
                            day.seconds = sec;
                            day.minutes = min;
                            char thisHour[2];
                            iota(day.hours, thisHour, 2);
                            char thisMin[2];
                            iota(day.minutes, thisMin, 2);
                            char thisSec[2];
                            iota(day.seconds, thisSec, 2);
                            xQueueOverwrite(Q_CLOCK, &day);
                            xQueuePeek(Q_CLOCK, &day, 10);
                            gfprintf(COM1, "\nTime of day set to: %02d:%02d:%02d\n\r", day.hours, day.minutes, day.seconds);
                        }
                        
                        
                    }
                    else if(InBuf[0] == '3' && i >= 5) // Set price on gas
                    {
                        struct gas_price currentPrice;
                        xQueuePeek(Q_GASPRICES, &currentPrice, 0);
                        if (InBuf[1] == '1')
                        {
                            FP32 LF92price = (((InBuf[2] - '0') * 1000.0f + (InBuf[3] - '0') * 100.0f + (InBuf[4] - '0') * 10.0f + (InBuf[5] - '0')) / 100.0f);
                            currentPrice.LF92_Price = LF92price + 0.0005;
                            xQueueOverwrite(Q_GASPRICES, &currentPrice);
                            bcd(currentPrice.LF92_Price, myarr);
                            gfprintf(COM1, "\n Lead free 92 price set to: %02d.%02d\n\r",  myarr[0], myarr[1]);
                        }
                        else if(InBuf[1] == '2')
                        {
                            FP32 LF95price = (((InBuf[2] - '0') * 1000.0f + (InBuf[3] - '0') * 100.0f + (InBuf[4] - '0') * 10.0f + (InBuf[5] - '0')) / 100.0f);
                            currentPrice.LF95_Price = LF95price + 0.0005;
                            xQueueOverwrite(Q_GASPRICES, &currentPrice);
                            bcd(currentPrice.LF95_Price, myarr);
                            gfprintf(COM1, "\n Lead free 95 price set to: %02d.%02d\n\r",  myarr[0], myarr[1]);
						}
                        else if(InBuf[1] == '3')
                        {
                            FP32 DIESEL =( ((InBuf[2] - '0') * 1000.0f + (InBuf[3] - '0') * 100.0f + (InBuf[4] - '0') * 10.0f + (InBuf[5] - '0')) / 100.0f);
                            currentPrice.DIESEL_Price = DIESEL + 0.0005;
                            xQueueOverwrite(Q_GASPRICES, &currentPrice);
                            bcd(currentPrice.DIESEL_Price, myarr);
                            gfprintf(COM1, "\n Diesel price set to: %02d.%02d\n\r",  myarr[0], myarr[1]);
						}

					}
                    else if (InBuf[0] == '4' && i > 1) // Display Data log Query
                    {
                        FP32 soldLD92 = 0;
                        FP32 soldLD95 = 0;
                        FP32 soldDI = 0;
                        FP32 gasolina = 0;
                        INT8U countCashPay = 0;
                        INT8U countCardPay = 0;
                        
                        struct gas_price currentPrice;
                        xQueuePeek(Q_GASPRICES, &currentPrice, 0);
                        for (int i = 0; i < purchase_counter; ++i)
                        {
                            
                            

                            if (purchase_log[i].product == S_LF92)
                            {
                                if (purchase_log[i].card_or_cash == S_CASH)
                                {
                                    soldLD92 += purchase_log[i].carNr_Or_Cash;
                                    ++countCashPay;
                                }
                                else
                                {
                                    gasolina = currentPrice.LF92_Price;
                                    soldLD92 += purchase_log[i].quantity * gasolina;
                                    ++countCardPay;
                                }
                             

                            }
                            else if (purchase_log[i].product == S_LF95)
                            {
                                if (purchase_log[i].card_or_cash == S_CASH)
                                {
                                    soldLD95 += purchase_log[i].carNr_Or_Cash;
                                    ++countCashPay;
                                }
                                else
                                {
                                    gasolina = currentPrice.LF95_Price;
                                    soldLD95 += purchase_log[i].quantity * gasolina;
                                    ++countCardPay;
                                }
                            }
                            else if (purchase_log[i].product == S_DIESEL)
                            {
                                if (purchase_log[i].card_or_cash == S_CASH)
                                {
                                    soldDI += purchase_log[i].carNr_Or_Cash;
                                    ++countCashPay;
                                }
                                else
                                {
                                    gasolina = currentPrice.DIESEL_Price;
                                    soldDI += purchase_log[i].quantity * gasolina;
                                    ++countCardPay;
                                }
                            }
                            
                            

                        }
                        gfprintf(COM1, "\nTotal card purchases %04d and Total cash purchases %04d\n\r", countCardPay, countCashPay);

                        bcd(soldLD92, myarr);
                        gfprintf(COM1, "\nTotal income from Lead Free 92: %06d.%02d DKK\r", myarr[0], myarr[1]);
                        bcd(soldLD95, myarr);
                        gfprintf(COM1, "\nTotal income from Lead Free 95: %06d.%02d DKK\r", myarr[0], myarr[1]);
                        bcd(soldDI, myarr);
                        gfprintf(COM1, "\nTotal income from Diesel: %06d.%02d DKK\r", myarr[0], myarr[1]);

                        FP32 total_sales = soldLD92 + soldLD95 + soldDI;
                        bcd(total_sales, myarr);
                        gfprintf(COM1, "\nTotal income from all gas types: %07d.%02d DKK\r", myarr[0], myarr[1]);

                        gfprintf(COM1, "\nTotal operating time of pump %06d sec\n\r", pump_operating_time);

                    }
                   
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
