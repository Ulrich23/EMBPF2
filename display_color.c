/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

/***************************** Include files *******************************/
#include "display_color.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
void display_color(INT16S color)
{
	INT64S clear = 0xF1; // 1111 0001

	GPIO_PORTF_DATA_R &= clear; // Clear portf from PF1 to PF3
	GPIO_PORTF_DATA_R |= color;
}
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/

/****************************** End Of Module *******************************/
