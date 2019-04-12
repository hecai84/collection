#include "dht.h"
//PA2
#define DHT_IO_IN() {DHT_GPIO_Port->CRL=0XFFFFF0FF;DHT_GPIO_Port->CRL|=0X00000800;}
#define DHT_IO_OUT() {DHT_GPIO_Port->CRL=0XFFFFF0FF;DHT_GPIO_Port->CRL|=0X00000300;}
#define DHT_H HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_SET)
#define DHT_L HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET)
#define DHT HAL_GPIO_ReadPin(DHT_GPIO_Port,DHT_Pin)

//变量定义
BYTE RH_data_H, RH_data_L, T_data_H,T_data_L;



BYTE COM(void)
{

    BYTE i,U8comdata,U8FLAG,U8temp ;
	
	for(i=0;i<8;i++)	   
	{
	
	    U8FLAG=2;	
		while((!DHT)&&U8FLAG++);  
		if(U8FLAG==1)break;
		delay_us(30);
		U8temp=0;
		if(DHT)
			U8temp=1;
		U8FLAG=2;
		while((DHT)&&U8FLAG++);
		//超时则跳出for循环		  
		if(U8FLAG==1)break;
		//判断数据位是0还是1	 		   
		// 如果高电平高过预定0高电平值则数据位为 1 		 
		U8comdata<<=1;
		U8comdata|=U8temp;        //0
	}//rof
	return  U8comdata	;
}


BOOL Read_DHT11()
{				  
	BYTE temp,check,flag;
	DHT_IO_OUT();
	DHT_L;
	HAL_Delay(18);
	//主机拉低18ms 
	DHT_H;
	//总线由上拉电阻拉高 主机延时20us
	delay_us(20);

	//主机设为输入 判断从机响应信号 
	//DQ=1;
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
	DHT_IO_IN();
	if(!DHT)		 //T !	  
	{
		flag=2;
		//判断从机是否发出 80us 的低电平响应信号是否结束	 
		while((!DHT)&&flag++);
		flag=2;
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while((DHT)&&flag++);
		//数据接收状态		 
		RH_data_H= COM();		
		RH_data_L= COM();		
		T_data_H=COM();
		T_data_L=COM();
		check=COM();
		
		DHT_IO_OUT();
		DHT_H;
		//数据校验 
		
		temp=(RH_data_H+RH_data_L+T_data_H+T_data_L);
		if(temp==check)
		{
		  return TRUE;
		}//fi
		return FALSE;	
	}
	return FALSE;
}

/*
********************************************************************************
** 函数名称 ： ReadHumidity(void)
** 函数功能 ： 获取湿度值
** 返回值:     温度127~-128
********************************************************************************
*/
BYTE ReadHumidity(void)
{
	return RH_data_H;
}
BYTE ReadTemperature(void)
{
	return T_data_H;
}
