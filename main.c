/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

// Systick Timer used for software TICK timer by FreeRTOS
// (used for context switching and time measuring)
#include "systick_frt.h"

// FreeRTOS include files
#include "FreeRTOS.h"   // IMPORTANT!!! Always include FreeRTOS.h before task.h or queue.h etc.
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Tasks
#include "tasktest.h" // (An example of a setup of a task, use this for reference)
#include "lcd.h"
#include "ui.h"
#include "key.h"
#include "clockTask.h"
#include "lcdMenuTask.h"
#include "paymentTask.h"
#include "drejimpulsTask.h"
#include "fuelingTask.h"
#include "uartTask.h"

//#include "fuelingTask.h"



// Display Color (For Debugging Purposes)
#include "display_color.h"

// Initialize GPIO pins for on board LEDs, Button Matrix and LCD.
#include "gpio.h"

// Others
#include "taskHandlers.h"
#include "queueHandlers.h"
#include "semaphoreHandlers.h"
#include "file.h"
#include "storage.h"




/*****************************    Defines    *******************************/
//#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  init_systick();       // Initialize Real time clock SystickTimer for Ticks
  init_gpio();          // Initialize GPIO pins for on board LEDs,
                        // Button Matrix and LCD.
  init_files();
  uart0_init( 9600, 8, 1, 'n' );
}


INT16U purchase_counter = 0; // Initialize the purchase counter global variable

int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{

  setupHardware();



  // Create the queues
  // ----------------

  Q_KEY = xQueueCreate(QUEUE_SIZE_KEY, sizeof(INT8U));
  Q_LCD = xQueueCreate(QUEUE_SIZE, sizeof(INT8U));
  Q_CLOCK = xQueueCreate( 1 , sizeof( struct time_day) );
  Q_PURCHASE = xQueueCreate(1, sizeof( struct purchase_state));
  //Q_DATA_LOG = xQueueCreate(QUEUE_MAX_PURCHASE, sizeof(data_log_Handle));
  Q_DREJIMPULS = xQueueCreate( 1 , sizeof(INT16U) );
  Q_GASPRICES = xQueueCreate( 1, sizeof(struct gas_price) );
  Q_FUELING_DISPLAY = xQueueCreate( 1, sizeof( FP32[2] ) );
  Q_UART_TX = xQueueCreate( QUEUE_SIZE, sizeof( INT8U ) );
  Q_UART_RX = xQueueCreate( QUEUE_SIZE, sizeof( INT8U ) );



  // Set the default price for the gas, can be changed later through the UART
  struct gas_price defualtPrice;
  defualtPrice.LF92_Price = 8.49f;                  
  defualtPrice.LF95_Price = 8.79f;
  defualtPrice.DIESEL_Price = 8.12f;

  

  xQueueOverwrite(Q_GASPRICES, &defualtPrice);


  // Create the semaphore
  // ----------------
  SEM_PURCHASE_QUEUE = xSemaphoreCreateMutex();
  //SEM_KEY_QUEUE = xSemaphoreCreateMutex();


  // Start the tasks.
  // ----------------
  // myTaskTest is an alive timer blinking green for 1 sec, then red for 1 sec.
  xTaskCreate(myTaskTest, "taskTest", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &myTaskTestHandle);

  xTaskCreate(lcd_Task, "lcdTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &lcdTaskHandle);
  xTaskCreate(key_Task, "keyTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &keyTaskHandle);
  xTaskCreate(clock_Task, "clockTask", configMINIMAL_STACK_SIZE, NULL, HIGH_PRIO, &clockTaskHandle);
  xTaskCreate(lcd_Menu_Display_Task, "lcdMenuDisplayTask", (unsigned short) 200, NULL, LOW_PRIO, &lcdMenuDisplayTaskHandle);
  xTaskCreate(lcd_Menu_Task, "lcdMenuTask", ( unsigned short ) 200 , NULL, LOW_PRIO, &lcdMenuTaskHandle);
  xTaskCreate(payment_Task, "paymentTask", ( unsigned short ) 200 , NULL, LOW_PRIO, &paymentTaskHandle);
  xTaskCreate(drejimpuls_Task, "drejimpulsTask", (unsigned short) 70, NULL, MED_PRIO, &drejimpulsTaskHandle);
  xTaskCreate(fueling_Task, "fuelingTask", (unsigned short) 200, NULL, LOW_PRIO, &fuelingTaskHandle);
  xTaskCreate(uart_rx_Task, "uartrxTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &uartrxTaskHandle);
  xTaskCreate(uart_tx_Task, "uarttxTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &uarttxTaskHandle);
  xTaskCreate(ui_Task, "uiTask", (unsigned short) 200, NULL, LOW_PRIO, &uiTaskHandle);



  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  display_color(MAGENTA);
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
