/*****************************************************************************

SDU Portfolio 2 [Final Assignment] Embedded Programming

Morten Efferbach Toft [motof15@student.sdu.dk]
Thobias Moldrup Sahi Aggerholm [thagg18@student.sdu.dk]
Ulrich Farhadi [ulfar18@student.sdu.dk]

*****************************************************************************/

#ifndef FILE_H_
#define FILE_H_

#include "emp_type.h"

typedef INT8U* FILE;

#define COM1  0      // UART
#define COM2  1      // LCD
#define COM3  2      // Keyboard
#define COM4  3      // Peek_Keyboard

BOOLEAN put_file( FILE, INT8U );

#endif /* FILE_H_ */
