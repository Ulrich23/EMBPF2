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

#define PIN_SIZE_KEY        4
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
  INT32U card_Nr_Val = 0;
  INT8U pin_Nr[PIN_SIZE_KEY] = {0};
  INT16U pin_Nr_Val = 0;

  while(1)
  {
    if (SEM_PURCHASE_QUEUE != NULL) // Should do an else statement where we use taskYIELD()
                                    // So we dont waste our cycles? here, since the semaphore will not
                                    // suddenly become available. Also taskYIELD() after each while loop
                                    // so a context switch doesnt happen while we are holding the semaphore
                                    // therefore stucking it?
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
            if(uxQueueSpacesAvailable( Q_KEY ) == 0) // Burde være 8 siden vi lige har clearet den i lcdMenuTask
                                                     // Load the newly 8 key presses into the card_Nr array
            {
              for (INT8U i = 0; i < QUEUE_SIZE_KEY; i++)
              {
                xQueueReceive(Q_KEY, &card_Nr[i], (TickType_t) 0);
              }
              xQueueReset( Q_KEY ); // FLUSH the queue with key values Q_KEY
              thisPurch.p_state = CARD_ACCEPTED;
            }



            break;

          case CARD_ACCEPTED:

              if(uxQueueSpacesAvailable( Q_KEY ) == 4)
              {

                for (INT8U i = 0; i < PIN_SIZE_KEY; i++)
                {
                  xQueueReceive(Q_KEY, &pin_Nr[i], (TickType_t) 0);
                }

                // Loop to calculate the card and pin numbers as integers
                for (INT8S i = QUEUE_SIZE_KEY-1, k = 1; i >= 0; i--, k*=10)
                {
                    if(i >= PIN_SIZE_KEY)
                    {
                        pin_Nr_Val += pin_Nr[i-PIN_SIZE_KEY]*k;
                    }
                    card_Nr_Val += card_Nr[i]*k;


                }


                if( (card_Nr_Val%2) == 0 && (pin_Nr_Val%2) != 0 ){
                    // This line below is a test and state needs to be changed!
                    thisPurch.p_state = CASH_PAYMENT; // Valid combination, go to next state

                }
                else if( (card_Nr_Val%2) != 0 && (pin_Nr_Val%2) == 0 ){
                    // This line below is a test and state needs to be changed!
                    thisPurch.p_state = CASH_PAYMENT; // Valid combination, go to next state

                }
                else{
                    // Reset purchase, reset the thisPurch object and go to state CHOOSE PAYMENT
                    // OR
                    // Set thispurch state to CARD PAYMENT, to genindtast the card_Nr and pin_Nr (i like this one)
                    // REMEMBER to reset the keyboard values xQueueReset( Q_KEY ); before, so the state is exactly as it was
                    // before.

                }


              }
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
