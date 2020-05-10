/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: storage.h(.h)
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

#ifndef FILES_STORAGE_H_
#define FILES_STORAGE_H_

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

/*****************************    Defines    *******************************/
enum purchaseStates
{
  CHOOSE_PAYMENT,
  CARD_PAYMENT,
  CARD_ACCEPTED,
  CASH_PAYMENT,
  CHOOSE_GAS,
  PUMPING,
  REFUELING_DONE,
  LOGGED
};

enum paymentMethod
{
  S_CASH,
  S_CARD
};

enum gasType
{
  S_LF92,
  S_LF95,
  S_DIESEL
};

struct time_day {
    INT8U seconds;
    INT8U minutes;
    INT8U hours;
};


struct purchase_log {
  struct time_day time_of_day;
  enum gasType product;
  INT16U   quantity;
  INT16U   operating_time;
  INT16U   total_price;
  enum paymentMethod card_or_cash;    //1 for card, 0 for cash
  enum purchaseStates p_state;
};




/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/


/****************************** End Of Module *******************************/


#endif /* FILES_STORAGE_H_ */
