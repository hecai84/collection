#include "uart.h"
//------------宏定义-------------------
#define DATA_LENGTH 20
#define DATA_HEAD 0xff
#define SendUart(dat) HAL_UART_Transmit(&huart1,&dat,1,0xffff)
//-------------------------------------
//------------变量定义-----------------
uint8_t sendbuffer[DATA_LENGTH];
uint8_t revbuffer[DATA_LENGTH];
TIM_HandleTypeDef htim3;
static int count=0;
static int recpos=0;
static uint8_t rebyte;
//-------------------------------------
//------------函数定义-----------------
void rec_succeed(int len);
BOOL checkDataCRC(void);
static void MX_TIM3_Init(void);
//-------------------------------------

//CRC-16表
uint16_t ptable[256] =
{
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 
0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, 
0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 
0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, 
0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 
0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D, 
0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 
0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC, 
0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 
0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B, 
0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 
0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 
0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 
0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 
0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 
0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, 
0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 
0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, 
0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 
0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, 
0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 
0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 
0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 
0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634, 
0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 
0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3, 
0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 
0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92, 
0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 
0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1, 
0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 
0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static void Error_Handler(void)
{
}

UART_HandleTypeDef huart1;
/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
uint16_t getCRC(uint8_t *dat,int len)
{


  uint16_t cRc_16 = 0;
  uint8_t temp;

  while(len-- > 0)
  {
    temp = cRc_16 >> 8; 
    cRc_16 = (cRc_16 << 8) ^ ptable[(temp ^ *dat++) & 0xFF];
  }

  return cRc_16; 
}


void initUartRecive(void)
{
	if(HAL_UART_Receive_IT(&huart1,(uint8_t *)&rebyte,1) != HAL_OK){    //这一句写在main函数的while(1)上面。用于启动程序启动一次中断接收
		//HAL_UART_Transmit(&huart1, (uint8_t *)&"ERROR\r\n",7,10);    
		while(1);
	} 
}

void process_rec(uint8_t byte)
{
	if(recpos==0)
	{
		if(byte==DATA_HEAD)
		{
			count=0;
			revbuffer[recpos++]=byte;
		}
	}else if(recpos==1)
	{
		if(byte>DATA_LENGTH-2)
		{
			recpos=0;
		}else
		{
			revbuffer[recpos++]=byte;
		}
	}else if(recpos<DATA_LENGTH)
	{
		revbuffer[recpos++]=byte;
		if(recpos>=revbuffer[1]+2)
		{
			recpos=0;
			rec_succeed(revbuffer[1]+2);
			
		}
	}else
	{
		recpos=0;
	}
	
}


/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7800-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

void rec_succeed(int len)
{
	if(checkDataCRC())
	{
		uint8_t temp[]={1,2,3,4,4};
		SendCmd(temp,5);
	}else
	{
		uint8_t temp[]={1,2,3,3,4};
		SendCmd(temp,5);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	//HAL_UART_Transmit(&huart1, (uint8_t *)&"\r\ninto HAL_UART_RxCpltCallback\r\n",32,0xffff);    //验证进入这个函数了
	//HAL_UART_Transmit(&huart1,(uint8_t *)&value,1,0xffff);//把接收到的数据通过串口发送出去       
	process_rec(rebyte);
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&rebyte,1);//重新打开串口中断
}	

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim3.Instance)
    {
        /* Toggle LED */
		if(count++>10)
			recpos=0;
		
    }
}


void InitUart(void)
{
	MX_TIM3_Init();
	HAL_TIM_Base_Start_IT(&htim3);
	MX_USART1_UART_Init();
	initUartRecive();
}
void sendarr(int len)
{
	int i;
	uint16_t key=getCRC(sendbuffer,len);
	uint8_t hi=(key>>8) & 0xff;	
	uint8_t lo=key & 0xff;
	for(i=0;i<len;i++)
	{
		HAL_UART_Transmit(&huart1,&sendbuffer[i],1,0xffff);//发送数据   
	}
	HAL_UART_Transmit(&huart1,&lo,1,0xffff);//发送数据  
	HAL_UART_Transmit(&huart1,&hi,1,0xffff);//发送数据    
}
void SendCmd(uint8_t * data,int len)
{
	int i;
	if(len <= DATA_LENGTH -2)
	{
		sendbuffer[0]=DATA_HEAD;
		sendbuffer[1]=len+2;
		for(i=0;i<len;i++)
		{
			sendbuffer[i+2]=data[i];
		}
		sendarr(len+2);
	}
}

//校验485数据 CRC校验
//返回值0为失败
//返回值1为成功
BOOL checkDataCRC(void)
{
	uint16_t tmp;
	if(revbuffer[0]!=DATA_HEAD)	//检查数据头
		return FALSE;
	else
	{
		int len= (int)(revbuffer[1]);	//获取数据长度
		if(len>4 && len<DATA_LENGTH-2)
		{
			tmp=getCRC(revbuffer,len);
			if(revbuffer[len]==(tmp&0xff) && revbuffer[len+1]==((tmp&0xff00) >> 8))
				return TRUE;
			else
				return FALSE;
		}
		return FALSE;
		
	}
}

void SendDebug(ULONG dat)
{
	uint8_t value;
	BYTE i=4;
	for(i=i;i>0;i--)
	{
		value=(dat>>((i-1)*8)) & 0xff;
		SendUart(value);
	}
}








