#ifndef __UART_H
#define __UART_H
#include "global.h"


#define DATA_LENGTH 20
#define DATA_HEAD 0xff

void InitUart(void);
void SendCmd(uint8_t * data,int len);
void SendDebug(ULONG dat);
BYTE GetUartCmd(void);
void ClearStatus(void);

#endif
