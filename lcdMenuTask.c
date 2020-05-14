/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: lcdMenuTask.c(.h)
*
* PROJECT....: EMBPF2
*
* DESCRIPTION: -
*
* Change Log:
******************************************************************************
* Date    Id    Change
* 8. maj 2020
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "lcdMenuTask.h"

/*****************************    Defines    *******************************/


enum gasStates
{
  LF92,
  LF95,
  DIESEL
};


enum gasStates gasState;


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void lcd_Menu_Task(void* p)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
  struct purchase_state thisPurch;
  struct purchase_state peekPurch;
  UBaseType_t ResponseQueue;

  INT8U gastypeSwitch = 0;


  //TickType_t myLastUnblock;
  //myLastUnblock = xTaskGetTickCount();

  while(1)
  {
	 //vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 1 ) );

     if (SEM_PURCHASE_QUEUE != NULL)
     {
       if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 0) == pdTRUE)  //Only if purchase qeueue is available
       {
		  if(uxQueueSpacesAvailable(Q_PURCHASE) == 1 )
			  {
			  thisPurch.p_state = CHOOSE_PAYMENT; // create a payment if no purchase is available.
			  }
          else
			  {
                xQueuePeek(Q_PURCHASE, &peekPurch, 0); //If done and logged start new refueling
                  if(peekPurch.p_state == LOGGED)
                  {
                    thisPurch.p_state = CHOOSE_PAYMENT; //Start purchase
                  }
                  else
                  {
                    xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t) 0); //Else get current purchase
                  }
          }

		  
          switch (thisPurch.p_state)
          {
            case CHOOSE_PAYMENT:
			  vTaskSuspend(fuelingTaskHandle);

              //vTaskResume(paymentTaskHandle);
              //vTaskSuspend(fuelingTaskHandle);
              if (get_star_key() == TRUE)
              {
                xQueueReset( Q_KEY );
                thisPurch.p_state = CARD_PAYMENT;
                thisPurch.card_or_cash = S_CARD;
                vTaskSuspend( drejimpulsTaskHandle );
                
                

              }
              else if (get_square_key() == TRUE)
              {
                xQueueReset( Q_KEY );
                vTaskResume( drejimpulsTaskHandle );

                thisPurch.p_state = CASH_PAYMENT;
                thisPurch.card_or_cash = S_CASH;
				
              }

              break;
            case CHOOSE_GAS:
				vTaskSuspend(drejimpulsTaskHandle);
                //vTaskSuspend(paymentTaskHandle);

            if (uxQueueSpacesAvailable( Q_KEY ) == 7)
            {

              xQueueReceive(Q_KEY, &gastypeSwitch, (TickType_t) 0);
              gastypeSwitch -= 48; // Convert ascii to decimal
              switch (gastypeSwitch) {
                case 1:
                thisPurch.product = S_LF92;
                thisPurch.p_state = FUELING;
                vTaskSuspend( myTaskTestHandle );
                break;
                case 2:
                thisPurch.product = S_LF95;
                thisPurch.p_state = FUELING;
				vTaskSuspend( myTaskTestHandle );
                break;
                case 3:
                thisPurch.product = S_DIESEL;
                thisPurch.p_state = FUELING;
				vTaskSuspend( myTaskTestHandle );
                break;
                default:
                break;
              }
            }
             break;
             case FUELING:
                 vTaskResume(fuelingTaskHandle);
                 
             break;

          }

          xQueueSendToFront(Q_PURCHASE, &thisPurch, 0);
          xSemaphoreGive(SEM_PURCHASE_QUEUE);
        }
        else{
          taskYIELD();
        }
      }




  }





}


void lcd_Menu_Display_Task(void *p)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
  TickType_t myLastUnblock;
  myLastUnblock = xTaskGetTickCount();
  struct purchase_state thisPurch;
  INT16U peekCounter = 0;
  struct gas_price currentPrice;
  INT16U myarr[2] = {0};
  FP32 myarr_fueling[2] = { 0 };


  INT8U gasdisplayer = 0; // Needs time to display gas types

  while(1)
  {

    vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 400 ) ); // Updating this task every 400ms

    //Look at state
    xQueuePeek(Q_PURCHASE, &thisPurch, 0);

    //Count the gas displayer;
    if (gasdisplayer < 9 && thisPurch.p_state == CHOOSE_GAS){
      gasdisplayer++;
    }
    else{
      gasdisplayer = 0;
    }


    switch (thisPurch.p_state)
    {
      case CHOOSE_PAYMENT:
        gfprintf( COM2, "%c%cFor card press *", 0x1B, 0x80);
        gfprintf( COM2, "%c%cFor cash press #", 0x1B, 0xC0);
        break;
      case CARD_PAYMENT:
        gfprintf( COM2, "%c%cEnter Card Nr   ", 0x1B, 0x80);
        gfprintf( COM2, "%c%c                ", 0x1B, 0xC0);
        //Buffer key

        break;
      case CARD_ACCEPTED:
        //gfprintf( COM2, "%c%cQueue Number:   ", 0x1B, 0x80);
        //gfprintf( COM2, "%c%c%c    ", 0x1B, 0xC0, 48 + uxQueueSpacesAvailable( Q_PURCHASE )); //uxQueueSpacesAvailable( Q_PURCHASE )
        gfprintf( COM2, "%c%cEnter PIN-code  ", 0x1B, 0x80);
        gfprintf( COM2, "%c%c                ", 0x1B, 0xC0);
        //Buffer key
        break;
      case CASH_PAYMENT:
		  
          //vTaskResume(drejimpulsTaskHandle);

		xQueuePeek(Q_DREJIMPULS, &peekCounter, 0);
        gfprintf( COM2, "%c%cTurn switch     ", 0x1B, 0x80);
        gfprintf( COM2, "%c%cDKK:%04d Enter #", 0x1B, 0xC0, peekCounter);
        //Buffer drejimpuls
        break;
      case CHOOSE_GAS:
          xQueuePeek(Q_GASPRICES, &currentPrice, 0);
		  
        switch (gasdisplayer)
        {
          case 3:  // These are in this specific order so it starts displaying (1), then (2), then (3)
			bcd(currentPrice.LF92_Price, myarr);
            gfprintf( COM2, "%c%cLeadfree 92  (1)", 0x1B, 0x80);
            if(myarr[0] >= 10){
                gfprintf( COM2, "%c%c%02d.%02d DKK/L     ", 0x1B, 0xC0, myarr[0], myarr[1]);
            }
            else{
                gfprintf( COM2, "%c%c%01d.%02d DKK/L      ", 0x1B, 0xC0, myarr[0], myarr[1]);
            }
            break;
          case 6:
			bcd(currentPrice.LF95_Price, myarr);
            gfprintf( COM2, "%c%cLeadfree 95  (2)", 0x1B, 0x80);
            if(myarr[0] >= 10){
                gfprintf( COM2, "%c%c%02d.%02d DKK/L     ", 0x1B, 0xC0, myarr[0], myarr[1]);
            }
            else{
                gfprintf( COM2, "%c%c%01d.%02d DKK/L      ", 0x1B, 0xC0, myarr[0], myarr[1]);
            }

            break;
          case 0:
			bcd(currentPrice.DIESEL_Price, myarr);
            gfprintf( COM2, "%c%cDiesel       (3)", 0x1B, 0x80);
            if(myarr[0] >= 10){
                gfprintf( COM2, "%c%c%02d.%02d DKK/L     ", 0x1B, 0xC0, myarr[0], myarr[1]);
            }
            else{
                gfprintf( COM2, "%c%c%01d.%02d DKK/L      ", 0x1B, 0xC0, myarr[0], myarr[1]);
            }

            break;
        }
      break;

      case FUELING:
		    gfprintf( COM2, "%c%cREMOVE NOZZLE   ", 0x1B, 0x80);
            gfprintf( COM2, "%c%cFuel ThatBitch %01d", 0x1B, 0xC0, get_button_id());
        
        //Buffer bars
        break;
      case NOZZLE_REMOVAL:
		    //Display amount of liters fueled, and total price of that
		    xQueuePeek(Q_FUELING_DISPLAY, &myarr_fueling, 0);
			bcd(myarr_fueling[1], myarr);
	        gfprintf( COM2, "%c%cLiters: %02d.%02d   ", 0x1B, 0x80, myarr[0], myarr[1]);

            bcd(myarr_fueling[0], myarr);
			gfprintf( COM2, "%c%cDKK: %03d.%02d     ", 0x1B, 0xC0, myarr[0], myarr[1]);
			break;

      case REFUELING_DONE:
        //Buffer Gas typedef
        //Buffer total price
        gfprintf( COM2, "%c%cFUCK OFF THEN   ", 0x1B, 0x80);
        gfprintf( COM2, "%c%c                ", 0x1B, 0xC0);
        break;
    }
  }
}


/****************************** End Of Module *******************************/
