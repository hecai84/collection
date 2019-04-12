#include "dht.h"
//PA2
#define DHT_IO_IN() {DHT_GPIO_Port->CRL=0XFFFFF0FF;DHT_GPIO_Port->CRL|=0X00000800;}
#define DHT_IO_OUT() {DHT_GPIO_Port->CRL=0XFFFFF0FF;DHT_GPIO_Port->CRL|=0X00000300;}
#define DHT_H HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_SET)
#define DHT_L HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET)
#define DHT HAL_GPIO_ReadPin(DHT_GPIO_Port,DHT_Pin)

//��������
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
		//��ʱ������forѭ��		  
		if(U8FLAG==1)break;
		//�ж�����λ��0����1	 		   
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 		 
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
	//��������18ms 
	DHT_H;
	//������������������ ������ʱ20us
	delay_us(20);

	//������Ϊ���� �жϴӻ���Ӧ�ź� 
	//DQ=1;
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
	DHT_IO_IN();
	if(!DHT)		 //T !	  
	{
		flag=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
		while((!DHT)&&flag++);
		flag=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		while((DHT)&&flag++);
		//���ݽ���״̬		 
		RH_data_H= COM();		
		RH_data_L= COM();		
		T_data_H=COM();
		T_data_L=COM();
		check=COM();
		
		DHT_IO_OUT();
		DHT_H;
		//����У�� 
		
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
** �������� �� ReadHumidity(void)
** �������� �� ��ȡʪ��ֵ
** ����ֵ:     �¶�127~-128
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
