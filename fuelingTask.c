/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: fuelingTask.c(.h)
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

#include "fuelingTask.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
void fueling_Task(void* p)
{
	struct purchase_state thisPurch;

	struct gas_price currentPrice;
	//Read only acces
	xQueuePeek(Q_GASPRICES, &currentPrice, 0);

	// SW1 = Nozzle
	// SW2 = Valve Lever

	FP32 total_Price = 0.0;  // measured in DKK
	FP32 gas_quantity = 0.0; // measured in liters

	while (1)
	{



		if (get_button_id() == CASE_SW1) // Nozzle is removed
		{
			if (SEM_PURCHASE_QUEUE != NULL) 
			{
				if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 0) == pdTRUE)
				{
					xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t) 0);
					//CASE_SW_DOUBLE;
					if (thisPurch.p_state == FUELING)
					{
						//thisPurch.total_price = total_Price;
						//thisPurch.quantity = gas_quantity;
						thisPurch.p_state = NOZZLE_REMOVAL;
					}

				xQueueSendToFront(Q_PURCHASE, &thisPurch, 0);
				xSemaphoreGive(SEM_PURCHASE_QUEUE);
				}
				else
				{
					taskYIELD();
				}
			}

			//...............
		}
		//puls 25 0.05 liter i 2 sek
		//puls 153 0.3 liter pr sek
		//puls 25 0.05 liter i 1 sek
	}

}
/****************************** End Of Module *******************************/
