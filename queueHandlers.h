#ifndef FILES_QUEUEHANDLERS_H_
#define FILES_QUEUEHANDLERS_H_

#include "FreeRTOS.h"
#include "queue.h"

  QueueHandle_t Q_KEY;
  QueueHandle_t Q_LCD;

  // Queuehandle for the 24hour clock (clockTask)
  QueueHandle_t Q_CLOCK;

  // Queuehandle for all purchases
  QueueHandle_t Q_PURCHASE;

/*****************************    Defines    *******************************/
#define QUEUE_SIZE          128
#define QUEUE_SIZE_KEY      8
#define QUEUE_MAX_PURCHASE  28 //Each purchase minimum takes 5 min


#endif /* FILES_QUEUEHANDLERS_H_ */
