#ifndef __BASE_H
#define __BASE_H
#include "stm32f1xx_hal.h"
#include "SEGGER_RTT.h"


//-------宏定义--------//
#define TRUE 			1
#define FALSE			0
#ifdef DEBUG
#define LOG(n, ...) SEGGER_RTT_printf(0,  #n "\n", ##__VA_ARGS__)
#else
#define LOG(n, ...)
#endif

//--声明类型--//
typedef unsigned char BYTE;
typedef unsigned short WORD; 
typedef unsigned char BOOL;
typedef unsigned int  UINT;
typedef unsigned long ULONG;


void delay_us(uint16_t us);
void InitTime(void);


#endif
