#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#include "display_color.h"



void display_color(INT16S color)
{
	INT64S clear = 0xF1; // 1111 0001

	GPIO_PORTF_DATA_R &= clear; // Clear portf from PF1 to PF3
	GPIO_PORTF_DATA_R |= color;
}