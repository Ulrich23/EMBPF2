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
  struct purchase_log thisPurch;
  struct purchase_log peekPurch;
  UBaseType_t ResponseQueue;

  while(1)
  {
     ResponseQueue = uxQueueSpacesAvailable( Q_PURCHASE );
     if(ResponseQueue == 0) // There can't be any more purchases today, (exeeded 288)
     {
       gfprintf( COM2, "%c%cToo Many        ", 0x1B, 0x80);
       gfprintf( COM2, "%c%cPurchases today ", 0x1B, 0xC0);
       vTaskSuspend ( NULL ); // Suspend the task itself.
     }

     if (SEM_PURCHASE_QUEUE != NULL)
     {
       if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 0))  //Only if purchase qeueue is available
       {
          xQueuePeek(Q_PURCHASE, &peekPurch, 0); //If previous refueling done and logged
          if(peekPurch.p_state == LOGGED)
          {
            thisPurch.p_state = CHOOSE_PAYMENT; //Start purchase
          }
          else
          {
            xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t) 0); //Else get current purchase
          }


          switch (thisPurch.p_state)
          {
            case CHOOSE_PAYMENT:
              if (get_star_key() == TRUE)
              {
                xQueueReset( Q_KEY );
                thisPurch.p_state = CARD_PAYMENT;
                thisPurch.card_or_cash = S_CARD;

              }
              else if (get_square_key() == TRUE)
              {
                thisPurch.p_state = CASH_PAYMENT;
                thisPurch.card_or_cash = S_CASH;
              }

              break;
            case CARD_PAYMENT:
             break;
          }

          xQueueSendToFront(Q_PURCHASE, &thisPurch, 0);
          xSemaphoreGive(SEM_PURCHASE_QUEUE);
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
  struct purchase_log thisPurch;


  while(1)
  {

    vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 400 ) ); // Updating this task every 200ms
    xQueuePeek(Q_PURCHASE, &thisPurch, 0);
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
        gfprintf( COM2, "%c%cTurn switch     ", 0x1B, 0x80);
        gfprintf( COM2, "%c%c                ", 0x1B, 0xC0);
        //Buffer drejimuls
        break;
      case CHOOSE_GAS:
        switch (gasState)
        {
          case LF92:
            gfprintf( COM2, "%c%cLeadfree 92     ", 0x1B, 0x80);
            gfprintf( COM2, "%c%cPrice: 8.49DKK/L", 0x1B, 0xC0);
            break;
          case LF95:
            gfprintf( COM2, "%c%cLeadfree 95     ", 0x1B, 0x80);
            gfprintf( COM2, "%c%cPrice: 8.79DKK/L", 0x1B, 0xC0);
            break;
          case DIESEL:
            gfprintf( COM2, "%c%cDiesel          ", 0x1B, 0x80);
            gfprintf( COM2, "%c%cPrice: 8.12DKK/L", 0x1B, 0xC0);
            break;
        }
      break;
      case PUMPING:
        gfprintf( COM2, "%c%cPumping...      ", 0x1B, 0x80);
        gfprintf( COM2, "%c%c                ", 0x1B, 0xC0);
        //Buffer bars
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
