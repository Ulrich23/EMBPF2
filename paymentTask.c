/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: paymentTask.c(.h)
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

#include "paymentTask.h"
/*****************************    Defines    *******************************/


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void payment_task(void* p)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
  struct purchase_log thisPurch;

  INT8U card_Nr[QUEUE_SIZE_KEY] = {0};

  while(1)
  {
    if (SEM_PURCHASE_QUEUE != NULL)
    {
      if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 0))
      {
        xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t) 0 );
        switch (thisPurch.p_state)
        {
          case CARD_PAYMENT:
            //look at keybuffer
            //if 8 wait 200 ms -> p_state = CARD_ACCEPTED
            //else nothing
            if(uxQueueSpacesAvailable( Q_KEY ) == 0)
            {
              for (INT8U i = 0; i < QUEUE_SIZE_KEY; i++)
              {
                xQueueReceive(Q_KEY, &card_Nr[i], (TickType_t) 0);
              }

              thisPurch.p_state = CARD_ACCEPTED;
            }




            break;

          case CARD_ACCEPTED:
            //Correct number and pin -> p_state = Choose_GAS
            //If not remove keybuffer -> p_state = CARD_PAYMENT
            break;
          case CASH_PAYMENT:

           break;
          default:
           break;
        }

        xQueueSendToFront(Q_PURCHASE, &thisPurch,0);
        xSemaphoreGive(SEM_PURCHASE_QUEUE);
      }
    }

  }
}


/****************************** End Of Module *******************************/
