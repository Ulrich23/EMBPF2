/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

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

  // Queuehandle for drejimpuls
  QueueHandle_t Q_DREJIMPULS;

  // Queuehandle for Gas prices
  QueueHandle_t Q_GASPRICES;

  // Queuehandle for data logs of previous purchases
  //QueueHandle_t Q_DATA_LOG;

  // Fueling display queue
  QueueHandle_t Q_FUELING_DISPLAY;

  // UART transmit queue
  QueueHandle_t Q_UART_TX;

  // UART receive queue
  QueueHandle_t Q_UART_RX;

/*****************************    Defines    *******************************/
#define QUEUE_SIZE          128
#define QUEUE_SIZE_KEY      8
#define QUEUE_MAX_PURCHASE  20 //Each purchase minimum takes 5 min


#endif /* FILES_QUEUEHANDLERS_H_ */
