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



	// SW1 = Nozzle
	// SW2 = Valve Lever

	FP32 fuelingAttr[2] = { 0.0 }; //Pos 0 total_price in DKK. Pos 2 gas quantity measured in liters.

	TickType_t myLastUnblock;
    myLastUnblock = xTaskGetTickCount();

	TickType_t myLastUnblock2;
    myLastUnblock2 = xTaskGetTickCount();

	while (1)
	{
		xQueuePeek(Q_PURCHASE, &peekPurch, 0);
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
				vTaskDelayUntil( &myLastUnblock , pdMS_TO_TICKS ( 10 ) ); // Debouncing since SW2 is held down
				
				    // Dette behøves ikke hvis vi laver det sekventielt, det er kun 0.3l/s der skal være i et while loop
							 // dette skal ændres til 0,3ml stadiet.
					if(get_button_id() == CASE_SW2) //Debounce
					{
						// Hvis der benyttes cash skal der først tjekkes om beløbet overhovedet kan holde til reduced start flow
						// dette skal implementeres i if statementet lige ovenover denne kommentar.
						display_color(YELLOW);
						Pulses = pulsPrSec(0.05f, PulsPrLiter, Pulses);
						vTaskDelayUntil(&myLastUnblock2, pdMS_TO_TICKS(1000));
						fuelingAttr[1] = Pulses;
						xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);
						display_color(YELLOW);
						Pulses = pulsPrSec(0.05f, PulsPrLiter, Pulses);
						vTaskDelayUntil(&myLastUnblock2, pdMS_TO_TICKS(1000));
						fuelingAttr[1] = Pulses;
						xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);
						

		

						while(get_button_id() == CASE_SW2) // Hvis SW2 stadig er holdt nede = fueling er i gang
														   // 0.3l per sec in this phaze
						{
							display_color(GREEN);
							Pulses = pulsPrSec(0.3f, PulsPrLiter, Pulses);
							vTaskDelayUntil(&myLastUnblock2, pdMS_TO_TICKS(1000));
							fuelingAttr[1] = Pulses;
							xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);

							// Hvis der benyttes cash skal der trækkes x antal moneys fra, hvis det overstiger beløbet BREAK

						}
						//display_color(YELLOW);
						//Pulses = pulsPrSec(0.05f, PulsPrLiter, Pulses);// Når SW2 slippes igen, = 0.05l/sek i 1 sek
						//vTaskDelayUntil(&myLastUnblock2, pdMS_TO_TICKS(1000));
						//fuelingAttr[1] = Pulses;
						//xQueueOverwrite(Q_FUELING_DISPLAY, &fuelingAttr);

						

				

						// afslut fueling.
						// Omregn liter til pulses/L
						// Gem de værdier der skal gemmes i et data_log objekt (tidsplunkt på dagen, quantity, antal penge tanket for)
						// send objektet til front i data_log queuen.
						// Sæt state til logged
						//break dette loop
						// denne task skal blockeres således at man ikke kan bruge Switchesne så husk at resume den når vi kommer
						// herind, og vtasksuspend (NULL) sig selv her i slutningen efter vi har breaket loopet.
						//.....................
			
				
					}
					

						
				
					
				

				

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
