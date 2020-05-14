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
  NOZZLE_REMOVAL,
  FUELING,
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


struct purchase_state {
  enum gasType product;
  INT16U cash_money_baby;
  enum paymentMethod  card_or_cash;    //1 for card, 0 for cash
  enum purchaseStates p_state;
};

struct data_log {
    struct time_day time_of_day;
    enum gasType product;
    FP32 quantity;
    INT16U carNr_Or_Cash;
    enum paymentMethod card_or_cash;    //1 for card, 0 for cash
};

typedef struct data_log* data_log_Handle;


struct gas_price {
    FP32 LF92_Price;
    FP32 LF95_Price;
    FP32 DIESEL_Price;
};

//typedef struct purchase_log *purchase_log_Handle;


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void bcd(FP32 fuel, INT16U arr[2]);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/


/****************************** End Of Module *******************************/


#endif /* FILES_STORAGE_H_ */
