#ifndef __UART_H
#define __UART_H
#include "global.h"

void InitUart(void);
void SendCmd(uint8_t * data,int len);
void SendDebug(ULONG dat);



#endif
