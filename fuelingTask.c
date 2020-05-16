/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

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
FP32 pulsPrSec(FP32 literPrSec, FP32 pulsPrL)
{
	FP32 res = 0.0f;
	res += (literPrSec) * (pulsPrL); //L/S * P/L -> P/S
	return res;

}
void fueling_Task(void* p)
{
	pump_operating_time = 0;

	struct purchase_state thisPurch;
	struct purchase_state peekPurch;

	struct gas_price currentPrice;
	//Read only acces
	xQueuePeek(Q_GASPRICES, &currentPrice, 10);

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

	vTaskSuspend(NULL);


	while (1)
	{
		counter_resume = 0; // Reset the counter
		xQueuePeek(Q_PURCHASE, &peekPurch, 10); // Peek the purchase to choose the gas price
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
		if (peekPurch.card_or_cash == S_CASH && peekPurch.p_state == FUELING)//SCAM?
		{
			if ((gas_price_current * 0.15f) >= peekPurch.cash_money_baby) // Make sure there is enough cash to complete
																		  // the Cost of first and last flow
			{
				if (SEM_PURCHASE_QUEUE != NULL)
				{
					if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 0) == pdTRUE) // if there is not enough cash go back to cash payment.
					{
						xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t) 0);
						thisPurch.p_state = CHOOSE_PAYMENT; // Reset the purchase (without logging it because it was invalid)
					
						xQueueSendToFront(Q_PURCHASE, &thisPurch, 0);
						xSemaphoreGive(SEM_PURCHASE_QUEUE);
					}
					taskYIELD(); // Don't waste time by looping in this task if the semaphore is taken by another task, just force a context switch
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
				//display_color(BLUE);
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
							display_color(RED_EMP);
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


			if ((get_button_id() == CASE_SW2) && !(fueling_state == logged_fueling))
			{
				//vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 10 ) ); // Debouncing since SW2 is held down
				vTaskDelay(10); // Debouncing
				    // Dette behoeves ikke hvis vi laver det sekventielt, det er kun 0.3l/s der skal vaere i et while loop
							 // dette skal aendres til 0,3ml stadiet.
				fueling_state = first_flow;

					while(!(fueling_state == logged_fueling))
					{
						if (get_button_id() == CASE_SW1) // Nozzle is removed
						{
							vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(5));
							while (get_button_id() == CASE_SW1)
							{
								taskYIELD();
								//display_color(BLUE);
							}
							nozzlePicked = !nozzlePicked;
							fueling_state = no_flow;

						}

						display_color(RED_EMP);
						if(!(get_button_id() == CASE_SW2) && (fueling_state == first_flow || fueling_state == regular_flow)) // Lever is released
						{
							fueling_state = last_flow; // goto last flow if lever is released.
						}
						


						switch (fueling_state)
						{
						case first_flow: // Checks on cash amount and fuels with 0.05L/sec for 2 seconds

							// enough cash? gas_price_current * 0.15f > peekPurch.cash_money_baby - fuelingattr[0]
							if(peekPurch.card_or_cash == S_CASH)
							{
								
								if ((gas_price_current * 0.15f) >= (peekPurch.cash_money_baby - fuelingAttr[0]) ) // is there enough money for first and last flow?
								{
									fueling_state = last_flow;
									break;
								}
							}


							for (INT8U i = 0; i < 2; ++i) //2 sec. i -> sec
							{
								display_color(YELLOW_EMP);
								Pulses += pulsPrSec(0.05f, PulsPrLiter); 
								vTaskDelay(1000);
								pump_operating_time += 1;
								fuelingAttr[1] = ((INT16U)Pulses) / 512.0f; // Liters
								fuelingAttr[0] = ((INT16U)Pulses) / PulsPrLiter * gas_price_current; // Price in DKK
								xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);
							}

							fueling_state = regular_flow;
							break;


						case regular_flow: // Checks on cash amount and keeps a flow on 0.3L/sec as long as there is enough cash or the lever is released

							if ((gas_price_current * 0.35f) >= (peekPurch.cash_money_baby - fuelingAttr[0]) && (peekPurch.card_or_cash == S_CASH)) // is there enough money for regular and last flow?
							{
								fueling_state = last_flow;
								break;
							}

							display_color(GREEN_EMP);
							Pulses += pulsPrSec(0.3f, PulsPrLiter);
							vTaskDelay(1000);
							pump_operating_time += 1;
							fuelingAttr[1] = ((INT16U)Pulses) / 512.0f; // Liters
							fuelingAttr[0] = ((INT16U)Pulses) / PulsPrLiter * gas_price_current; // Price in DKK
							xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);

							break;

						case last_flow: // Fuels with 0.05L/sec for 1 second
							
							display_color(YELLOW_EMP);
							Pulses += pulsPrSec(0.05f, PulsPrLiter);
							vTaskDelay(1000);
							pump_operating_time += 1;
							fuelingAttr[1] = ((INT16U)Pulses) / 512.0f; // Liters
							fuelingAttr[0] = ((INT16U)Pulses) / PulsPrLiter * gas_price_current; // Price in DKK
							xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);

							fueling_state = no_flow; //mofo

							
							break;
						case no_flow:
							
							display_color(RED_EMP); // Display color representing no flow

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
							if(counter_resume > 1500) // Checks if 15 seconds has passed after lever release
							{
								fueling_state = logged_fueling;
								nozzlePicked = 0;

							}
							if (nozzlePicked == 0) // Checks if the nozzle is put back
							{
								fueling_state = logged_fueling;
							}


							break;
						
						}

						


					}
					

					// logging...
					// Reset fueling_state to no_flow
					// SUSPEND THIS TASK
					// Reset nozzlePicked = 0;


			}





		}
		// LOGGING
		xQueuePeek(Q_PURCHASE, &peekPurch, 10);
		if ((peekPurch.p_state == NOZZLE_REMOVAL) && (nozzlePicked == 0))
		//if ((peekPurch.p_state == NOZZLE_REMOVAL) && (fueling_state = logged_fueling))
		{
			struct data_log thisLog;
			xQueuePeek(Q_CLOCK, &thisLog.time_of_day, 10);
			
			thisLog.product = peekPurch.product; // log chosen product
			thisLog.quantity = fuelingAttr[1]; // log quantity of chosen product
			if (peekPurch.card_or_cash == S_CASH)
			{
				thisLog.carNr_Or_Cash = peekPurch.cash_money_baby;
			}
			else
			{
				thisLog.carNr_Or_Cash = peekPurch.cash_money_baby; //Is card number if card_or_cash == S_CARD
			}
			
			thisLog.card_or_cash = peekPurch.card_or_cash;

			put_purchase_data(thisLog); //log data

			if (SEM_PURCHASE_QUEUE != NULL) //Update this purchase state
			{
			    while(1) // loop here until the semaphore becomes available
			    {
			        if(uxSemaphoreGetCount(SEM_PURCHASE_QUEUE) == 1) //returns 1 if semaphore is available.
			        {
			            break;
			        }
			    }
				if (xSemaphoreTake(SEM_PURCHASE_QUEUE, 10) == pdTRUE)
				{
					xQueueReceive(Q_PURCHASE, &thisPurch, (TickType_t) 0);
					//CASE_SW_DOUBLE;
					
					thisPurch.p_state = CHOOSE_PAYMENT;
					thisPurch.cash_money_baby = 0;
					INT16U temp = 0;
					xQueueOverwrite(Q_DREJIMPULS, &temp);
					
					xQueueSendToFront(Q_PURCHASE, &thisPurch, 0);
					xSemaphoreGive(SEM_PURCHASE_QUEUE);
					
				}

			}
			// Resetting all values declared at the start of this task, so it will be the same for the next fuelling session
			counter_resume = 0;
			Pulses = 0.0;
			fueling_state = no_flow;
			fuelingAttr[0] =  0.0 ; 
			fuelingAttr[1] =  0.0 ;
			xQueueReset( Q_KEY );
			vTaskSuspend(NULL);
		}

		

		
		
		// logging... (yes)
		// Reset fueling_state to no_flow (yes)
		// SUSPEND THIS TASK (yes)
		// Reset nozzlePicked = 0; (yes)



		//1,953125 mL pr puls
		//0,512 puls pr mL


		//puls 25 0.05 liter i 2 sek
		//puls 153 0.3 liter pr sek
		//puls 25 0.05 liter i 1 sek
	}

}
/****************************** End Of Module *******************************/
