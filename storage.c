/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: storage.c(.h)
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
#include "storage.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
//data_log_pool.counter = 0;
/*****************************   Functions   *******************************/

void put_purchase_data(struct data_log data)
{
    purchase_log[purchase_counter] = data;
    purchase_counter++;
    if (purchase_counter == QUEUE_MAX_PURCHASE)
    {
        purchase_counter = 0;
    }
}

//struct data_log* prvGetBuffer(struct dataLOG_BufferPOOL* bufPool)
//{
//    INT16U count = (*bufPool).counter;
//    struct data_log data* = &((*bufPool).pool[count]);
//    (*bufPool).counter = count + 1;
//    if (count == QUEUE_MAX_PURCHASE)
//    {
//        (*bufPool).counter = 0;
//    }
//    return data;
//}

void bcd(FP32 fuel, INT16U arr[2])
{

   // 257.18*100 = 25718
     //   25718/100 = 257
       // 25718%100 = 18
    // fuel = 11.24
    FP32 fuel_int = fuel * 100;
    INT16U temp = (int) fuel_int;
    // fuel_int = 1124
    arr[0] = temp/100; // = 11

    arr[1] = temp%100; // = 24
}
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/

/****************************** End Of Module *******************************/





