/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: lcdMenuTask.c(.h)
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
#include "lcdMenuTask.h"

/*****************************    Defines    *******************************/
enum menuStates
{
  CHOOSE_PAYMENT,
  CARD_PAYMENT,
  CARD_ACCEPTED,
  CASH_PAYMENT,
  CHOOSE_GAS,
  PUMPING,
  REFUELING_DONE
};

enum gasStates
{
  LF92,
  LF95,
  DIESEL
};


enum menuStates menuState = CHOOSE_PAYMENT;
enum gasStates gasState;


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void lcd_Menu_Task(void* p)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
  while(1)
  {

    struct purchase_log thisPurch;

    switch (menuState)
    {
      case CHOOSE_PAYMENT:
        if (get_star_key() == TRUE)
        {
          menuState = CARD_PAYMENT;
          thisPurch.card_or_cash = S_CARD;
        }
        else if (get_square_key() == TRUE)
        {
          menuState = CASH_PAYMENT;
          thisPurch.card_or_cash = S_CASH;
        }
        break;
      case CARD_PAYMENT:
      break;
    }
  }


}


void lcd_Menu_Display_Task(void *p)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{


  while(1)
  {
    switch (menuState)
    {
      case CHOOSE_PAYMENT:
        gfprintf( COM2, "%c%cFor card press *", 0x1B, 0x80);
        gfprintf( COM2, "%c%cFor cash press #", 0x1B, 0xC0);
        break;
      case CARD_PAYMENT:
        gfprintf( COM2, "%c%cEnter Card Nr   ", 0x1B, 0x80);
        //Buffer key
        break;
      case CARD_ACCEPTED:
        gfprintf( COM2, "%c%cEnter PIN-code  ", 0x1B, 0xC0);
        //Buffer key
        break;
      case CASH_PAYMENT:
        gfprintf( COM2, "%c%cTurn switch     ", 0x1B, 0x80);
        //Buffer drejimuls
        break;
      case CHOOSE_GAS:
        switch (gasState)
        {
          case LF92:
            gfprintf( COM2, "%c%cLeadfree 92     ", 0x1B, 0x80);
            gfprintf( COM2, "%c%cPrice: 8.49DKK/L", 0x1B, 0xC0);
            break;
          case LF95:
            gfprintf( COM2, "%c%cLeadfree 95     ", 0x1B, 0x80);
            gfprintf( COM2, "%c%cPrice: 8.79DKK/L", 0x1B, 0xC0);
            break;
          case DIESEL:
            gfprintf( COM2, "%c%cDiesel          ", 0x1B, 0x80);
            gfprintf( COM2, "%c%cPrice: 8.12DKK/L", 0x1B, 0xC0);
            break;
        }
      break;
      case PUMPING:
        gfprintf( COM2, "%c%cPumping...      ", 0x1B, 0x80);
        //Buffer bars
        break;
      case REFUELING_DONE:
        //Buffer Gas typedef
        //Buffer total price
        gfprintf( COM2, "%c%cFUCK OFF THEN   ", 0x1B, 0x80);
    }
  }
}


/****************************** End Of Module *******************************/
