/*****************************************************************************

SDU Portfolio 2 Embedded Programming

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
}



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

  Q_KEY = xQueueCreate(QUEUE_SIZE, sizeof(INT8U));
  Q_LCD = xQueueCreate(QUEUE_SIZE, sizeof(INT8U));
  Q_CLOCK = xQueueCreate( 1 , sizeof( struct time_day * ) );
  Q_PURCHASE = xQueueCreate(QUEUE_MAX_PURCHASE, sizeof( struct purchase_log * ));

  // Create the semaphore
  // ----------------



  // Start the tasks.
  // ----------------
  xTaskCreate(myTaskTest, "taskTest", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &myTaskTestHandle);
  xTaskCreate(lcd_task, "lcdTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &lcdTaskHandle);
  xTaskCreate(key_task, "keyTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &keyTaskHandle);
  xTaskCreate(clock_task, "clockTask", configMINIMAL_STACK_SIZE, NULL, HIGH_PRIO, &clockTaskHandle);
  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
