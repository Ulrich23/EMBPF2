/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: switch_id.c(.h)
*
* PROJECT....: Final_assignment_EMB
*
* DESCRIPTION: -
*
* Change Log:
******************************************************************************
* Date    Id    Change
* 12 May 2020
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#include "switch_id.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
INT16S get_button_id(){

    INT16S case_ID = 0;
    // Remember (Bit 0 = SW2 , Bit 4 = SW1)
    if(!(GPIO_PORTF_DATA_R & 0x10)) { // (0001 0000). SW1 is pressed. (Remember when inputs are active, they are 0)
                                      // so when switch is pressed, its value is 0.
        case_ID = CASE_SW1;
    }
    else if(!(GPIO_PORTF_DATA_R & 0x01)) { // SW2 is pressed.
        case_ID = CASE_SW2;
    }

    return case_ID;
}
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/

/****************************** End Of Module *******************************/





