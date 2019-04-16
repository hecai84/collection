#ifndef __BASE_H
#define __BASE_H
#include "stm32f1xx_hal.h"
#include "SEGGER_RTT.h"

//-------宏定义--------//
#define TRUE 			1
#define FALSE			0
#define LOG(format, ...) SEGGER_RTT_printf(0, format, __VA_ARGS__)

//--声明类型--//
typedef unsigned char BYTE;
typedef unsigned short WORD; 
typedef unsigned char BOOL;
typedef unsigned int  UINT;
typedef unsigned long ULONG;


void delay_us(uint16_t us);
void InitTime(void);


#endif
