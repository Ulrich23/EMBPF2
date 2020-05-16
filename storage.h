/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef FILES_STORAGE_H_
#define FILES_STORAGE_H_

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

/********************************Variables**********************************/
INT64U pump_operating_time;
INT16U purchase_counter;

#include "FreeRTOS.h"
#include "queueHandlers.h"
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


struct purchase_state { // Struct to keep data on the puchase
  enum gasType product;
  INT32U cash_money_baby;  // This variable also holds the card nr if card = 1 is chosen
  enum paymentMethod  card_or_cash;    //1 for card, 0 for cash
  enum purchaseStates p_state;
};

struct data_log {
    struct time_day time_of_day;
    enum gasType product;
    FP32 quantity;
    INT32U carNr_Or_Cash; // The card number or cash amount is stored here
    enum paymentMethod card_or_cash;    //1 for card, 0 for cash
};

struct data_log purchase_log[QUEUE_MAX_PURCHASE];

void put_purchase_data(struct data_log);

//typedef struct data_log* data_log_Handle;


struct gas_price {
    FP32 LF92_Price;
    FP32 LF95_Price;
    FP32 DIESEL_Price;
};

//typedef struct purchase_log *purchase_log_Handle;

//struct dataLOG_BufferPOOL
//{
//    struct data_log pool[QUEUE_MAX_PURCHASE];
//    INT16U counter;
//}

//struct dataLOG_BufferPOOL data_log_pool; // creat an object of the BufferPOOL


//struct data_log* prvGetBuffer(struct dataLOG_BufferPOOL* bufPool);

//  return buffer to get.
//  }
//struct dataLOG_BufferPOOL somename;

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
