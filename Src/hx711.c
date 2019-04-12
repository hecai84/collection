#include "hx711.h"
#include "uart.h"
#define Sck1_H HAL_GPIO_WritePin(Sck1_GPIO_Port, Sck1_Pin, GPIO_PIN_SET)
#define Sck1_L HAL_GPIO_WritePin(Sck1_GPIO_Port, Sck1_Pin, GPIO_PIN_RESET)
#define Dt1 HAL_GPIO_ReadPin(Dt1_GPIO_Port,Dt1_Pin)



//****************************************************
//读取HX711
//****************************************************
ULONG HX711_Read(void)	//增益128
{
	ULONG count; 
	UINT i; 
//	HX711_DOUT=1; 
//	delay_ms(1);
  	Sck1_L; 
  	count=0; 
	for(i=0;i<50;i++)
	{
		if(!Dt1)
			break;
		else
			HAL_Delay(10);
	}
	if(Dt1)
		return 0;


  	for(i=0;i<24;i++)
	{ 
	  	Sck1_H; 
	  	count=count<<1; 
		Sck1_L; 
	  	if(Dt1)
			count++; 
	} 
 	Sck1_H; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	HAL_Delay(10);
	Sck1_L;  
	SendDebug(count);
	return(count);
}
