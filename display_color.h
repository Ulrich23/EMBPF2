/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef FILES_DISPLAY_COLOR_H_
#define FILES_DISPLAY_COLOR_H_

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"


/*****************************    Defines    *******************************/
#define OFF		0
#define RED		2
#define BLUE	4
#define MAGENTA 6
#define GREEN	8
#define YELLOW	10
#define CYAN	12
#define WHITE	14

#define RED_EMP			12
#define YELLOW_EMP		10
#define REDYELLOW_EMP	8
#define GREEN_EMP		6
#define OFF_EMP			14

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/



void display_color(INT16S color);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/


/****************************** End Of Module *******************************/


#endif /* FILES_DISPLAY_COLOR_H_ */
