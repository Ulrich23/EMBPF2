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
enum fueling_progress
{
	first_flow,
	regular_flow,
	last_flow,
	wait_5_sec,
	no_flow,
	logged_fueling
};
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
FP32 pulsPrSec(FP32 literPrSec, FP32 pulsPrL, FP32 quant)
{
	FP32 res = quant;
	res += (literPrSec) * (pulsPrL); //L/S * P/L -> P/S
	return res;

}
void fueling_Task(void* p)
{
	struct purchase_state thisPurch;
	struct purchase_state peekPurch;

	struct gas_price currentPrice;
	//Read only acces
	xQueuePeek(Q_GASPRICES, &currentPrice, 0);

	BOOLEAN nozzlePicked = 0;
	
	
	FP32 PulsPrLiter = 512;
	FP32 Pulses = 0.0f;
	FP32 gas_price_current = 0.0f;

	enum fueling_progress fueling_state = no_flow;

	INT16U counter_resume = 0;


	// SW1 = Nozzle
	// SW2 = Valve Lever

	FP32 fuelingAttr[2] = { 0.0 }; //Pos 0 total_price in DKK. Pos 2 gas quantity measured in liters.

	TickType_t myLastUnblock;
    myLastUnblock = xTaskGetTickCount();

	TickType_t myLastUnblock2;
    myLastUnblock2 = xTaskGetTickCount();

	while (1)
	{
		counter_resume = 0;
		xQueuePeek(Q_PURCHASE, &peekPurch, 0);
		switch (peekPurch.product)
		{
			case S_LF92:
				gas_price_current = currentPrice.LF92_Price;
				break;
			case S_LF95:
				gas_price_current = currentPrice.LF95_Price;
				break;
			case S_DIESEL:
				gas_price_current = currentPrice.DIESEL_Price;
				break;
		}
		if (peekPurch.card_or_cash == S_CASH)//SCAM?
		{
			if ((gas_price_current * 0.15f) > peekPurch.cash_money_baby) //Cost of first and last flow
			{
				if (SEM_PURCHASE_QUEUE != NULL)
				{
					if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 1000) == pdTRUE) // if there is not enough cash go back to cash payment.
					{
						xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t) 0);
						thisPurch.p_state = CHOOSE_PAYMENT;
					
						xQueueSendToFront(Q_PURCHASE, &thisPurch, 0);
						xSemaphoreGive(SEM_PURCHASE_QUEUE);
					}
					taskYIELD();
				}
			}
		}


		//vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 5 ) );

		if (get_button_id() == CASE_SW1) // Nozzle is removed
		{
			vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 5 ) );
			while (get_button_id() == CASE_SW1)
			{
				taskYIELD();
			}
			nozzlePicked = !nozzlePicked;
			
		}
		if ((nozzlePicked == 1) )
		{
			if (peekPurch.p_state == FUELING)
			{
				if (SEM_PURCHASE_QUEUE != NULL)
				{
					if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 0) == pdTRUE)
					{
						xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t)0);
						//CASE_SW_DOUBLE;
						if (thisPurch.p_state == FUELING)
						{
							thisPurch.p_state = NOZZLE_REMOVAL;
							display_color(RED);
						}

						xQueueSendToFront(Q_PURCHASE, &thisPurch, 0);
						xSemaphoreGive(SEM_PURCHASE_QUEUE);
					}
					else
					{
						taskYIELD();
					}
				}
			}


			xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);


			if ((get_button_id() == CASE_SW2) && (peekPurch.p_state == NOZZLE_REMOVAL))
			{
				//vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 10 ) ); // Debouncing since SW2 is held down
				vTaskDelay(10); // Debouncing
				    // Dette behøves ikke hvis vi laver det sekventielt, det er kun 0.3l/s der skal være i et while loop
							 // dette skal ændres til 0,3ml stadiet.
				fueling_state = first_flow;

					while(!(fueling_state == logged_fueling))
					{
						display_color(RED);
						if(!(get_button_id() == CASE_SW2) && (fueling_state == first_flow || fueling_state == regular_flow))
						{
							fueling_state = last_flow;
						}
						


						switch (fueling_state)
						{
						case first_flow:

							// enough cash? gas_price_current * 0.15f > peekPurch.cash_money_baby - fuelingattr[0]
							if(peekPurch.card_or_cash == S_CASH)
							{
								if ((gas_price_current * 0.15f) > (peekPurch.cash_money_baby - fuelingAttr[0]) ) // is there enough money for first and last flow?
								{
									fueling_state = last_flow;
									break;
								}
							}

							for (INT8U i = 0; i < 2; ++i) //2 sec. i -> sec
							{
								display_color(YELLOW);
								Pulses = pulsPrSec(0.05f, PulsPrLiter, Pulses);
								vTaskDelay(1000);
								fuelingAttr[1] += ((INT16U)Pulses) / 512.0f; // Liters
								fuelingAttr[0] += ((INT16U)Pulses) / PulsPrLiter * gas_price_current; // Price in DKK
								xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);
							}

							fueling_state = regular_flow;
							break;


						case regular_flow:

							if ((gas_price_current * 0.35f) > (peekPurch.cash_money_baby - fuelingAttr[0]) && (peekPurch.card_or_cash == S_CASH)) // is there enough money for first and last flow?
							{
								fueling_state = last_flow;
								break;
							}

							display_color(GREEN);
							Pulses = pulsPrSec(0.3f, PulsPrLiter, Pulses);
							vTaskDelay(1000);
							fuelingAttr[1] += ((INT16U)Pulses / 512.0f; // Liters
							fuelingAttr[0] += ((INT16U)Pulses / PulsPrLiter * gas_price_current; // Price in DKK
							xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);

							break;

						case last_flow:
							
							display_color(YELLOW);
							Pulses = pulsPrSec(0.05f, PulsPrLiter, Pulses);
							vTaskDelay(1000);
							fuelingAttr[1] += ((INT16U)Pulses) / 512.0f; // Liters
							fuelingAttr[0] += ((INT16U)Pulses) / PulsPrLiter * gas_price_current; // Price in DKK
							xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);

							fueling_state = no_flow; //mofo

							
							break;
						case no_flow:
							
							display_color(RED); //WE DONE BABY

							vTaskDelay(10);
							counter_resume++;

							if(peekPurch.card_or_cash == S_CARD)
							{
								if((get_button_id() == CASE_SW2) && (counter_resume < 500) )
								{
									fueling_state = first_flow;
									counter_resume = 0;

								}
							}
							if(counter_resume > 1500)
							{
								fueling_state = logged_fueling;
							}

							break;
						
						}

						


					}
					display_color(BLUE);
					// logging...
					// Reset fueling_state to no_flow and SUSPEND THIS TASK



							
				
					
				

				

			}


		}

		//1,953125 mL pr puls
		//0,512 puls pr mL


		//puls 25 0.05 liter i 2 sek
		//puls 153 0.3 liter pr sek
		//puls 25 0.05 liter i 1 sek
	}

}
/****************************** End Of Module *******************************/
