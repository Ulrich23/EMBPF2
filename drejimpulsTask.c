/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: drejimpulsTask.c(.h)
*
* PROJECT....: EMBPF2
*
* DESCRIPTION: -
*
* Change Log:
******************************************************************************
* Date    Id    Change
* 12. maj 2020
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "drejimpulsTask.h"

// TO be deleted
//#include "storage.h"
//#include "ui.h"
//#include "string.h"
//#include "file.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
void drejimpuls_Task(void *p)
{

	INT16U counter = 0;


	INT8U AB0[2] = { !(GPIO_PORTA_DATA_R & 0x20), !(GPIO_PORTA_DATA_R & 0x40) };
	INT8U AB1[2] = { !(GPIO_PORTA_DATA_R & 0x20), !(GPIO_PORTA_DATA_R & 0x40) };

	INT8U YY[2] = { 0,0 };

	TickType_t myLastUnblock;
    myLastUnblock = xTaskGetTickCount();



	while (1)
	{

		vTaskDelayUntil(&myLastUnblock, pdMS_TO_TICKS(1)); // This task wakes up every 5ms
																 // and runs its code
															// It is suspended and resumes every 5 ms.
		
		//read pushDown on digiswitch						
		if (!(GPIO_PORTA_DATA_R & 0x80))
		{
			counter = 0;
		}										 


		AB0[0] = !(GPIO_PORTA_DATA_R & 0x20);
		AB0[1] = !(GPIO_PORTA_DATA_R & 0x40);

		if (AB1 != AB0)
		{
			YY[0] = AB1[0] ^ AB0[0];
			YY[1] = AB1[1] ^ AB0[1];

			if (AB1[0] == AB1[1])
			{
				if (YY[0] == 0 && YY[1] == 1)
				{
					counter += 5;
				}
				else if (YY[0] == 1 && YY[1] == 0)
				{
					counter += 50;

				}

			}
			else
			{
				if (YY[0] == 1 && YY[1] == 0)
				{
					counter += 5;
				}
				else if(YY[0] == 0 && YY[1] == 1)
				{
					counter += 50;
				}
			}

			AB1[0] = AB0[0];
			AB1[1] = AB0[1];


		}


		xQueueOverwrite(Q_DREJIMPULS, &counter);
	}
};
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/

/****************************** End Of Module *******************************/
