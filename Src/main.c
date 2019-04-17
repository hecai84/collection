/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hx711.h"
#include "uart.h"
#include "dht.h"
#include "door.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern BYTE revbuffer[];
BYTE sendData[DATA_LENGTH];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
static void processUart(void);
static void cmd2(BYTE arg1);
static void cmd4(BYTE arg1);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief 处理串口接收到的数据
  * @retval None
  */
static void processUart(void)
{
	BYTE cmd=GetUartCmd();
	BYTE arg1=revbuffer[3];
	if(cmd)
	{
		LOG("%02X%02X%02X%02X%02X%02X%02X\n",revbuffer[0],revbuffer[1],revbuffer[2],revbuffer[3],revbuffer[4],revbuffer[5],revbuffer[6]);
		switch(cmd)
		{
			case 1:
				
				break;
			case 2:
				cmd2(arg1);
				break;
			case 3:
				break;
			case 4:
				cmd4(arg1);
				break;
		
		
		
		}
		//清除状态
		ClearStatus();
	}
}

static void cmd2(BYTE arg1)
{
	if(arg1==2)
	{
		if(OpenDoor())
		{
			sendData[0]=0x12;
			sendData[1]=0x02;
		}else
		{
			sendData[0]=0x12;
			sendData[1]=0x03;
		}
		SendCmd(sendData,2);
	}else if(arg1==3)
	{
		if(CloseDoor())
		{
			sendData[0]=0x12;
			sendData[1]=0x06;
		}else
		{
			sendData[0]=0x12;
			sendData[1]=0x07;
		}
		SendCmd(sendData,2);
	}
}
static void cmd2re(DOOR_State state)
{
	if(state==DOORSTATE_DOOROPENED)
	{
		sendData[0]=0x12;
		sendData[1]=0x04;
	}else if(state==DOORSTATE_DOORCLOSED)
	{
		sendData[0]=0x12;
		sendData[1]=0x05;
	}else if(state==DOORSTATE_CLOSED)
	{
		sendData[0]=0x12;
		sendData[1]=0x06;
	}
	SendCmd(sendData,2);
}

static void cmd4(BYTE arg1)
{	
	if(arg1==3)
	{
		sendData[0]=4;
		sendData[1]=3;
		if(Read_DHT11())
		{
			sendData[2]=ReadHumidity();
			sendData[3]=ReadTemperature();
		}else
		{
			sendData[2]=0xee;
			sendData[3]=0xee;
		}
		SendCmd(sendData,4);					
			
	}
}

static void checkDoor()
{
	int count=30;
	DOOR_State state=GetCurState();
	if(state==DOORSTATE_LOCKOPENED)
	{
		//锁已经打开，判断门状态
		if(CheckDoorOpened())
		{
			//门已经打开
			cmd2re(DOORSTATE_DOOROPENED);
		}
	}else if(state==DOORSTATE_DOOROPENED)
	{
		//门已经打开，判断关门状态
		if(CheckDoorClosed())
		{
			//门已经关闭
			cmd2re(DOORSTATE_DOORCLOSED);
		}		
	}else if(state==DOORSTATE_DOORCLOSED)
	{
		//门已经关闭，判断是否重新打开
		while(count--)
		{
			HAL_Delay(100);
			//锁已经打开，判断门状态
			if(CheckDoorOpened())
			{
				//门已经打开
				cmd2re(DOORSTATE_DOOROPENED);
				return;
			}
		}
		CloseDoor();
		cmd2re(DOORSTATE_CLOSED);
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	InitTime();
	InitUart();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  //cmd4(3);
	  //HAL_Delay(2000);
		processUart();
		checkDoor();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DHT_Pin|Sck1_Pin|Sck2_Pin|Sck3_Pin 
                          |Sck4_Pin|Sck5_Pin|Sck6_Pin|Sck7_Pin 
                          |Sck8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ButtonLed_GPIO_Port, ButtonLed_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : DHT_Pin Sck1_Pin Sck2_Pin Sck3_Pin 
                           Sck4_Pin Sck5_Pin Sck6_Pin Sck7_Pin 
                           Sck8_Pin */
  GPIO_InitStruct.Pin = DHT_Pin|Sck1_Pin|Sck2_Pin|Sck3_Pin 
                          |Sck4_Pin|Sck5_Pin|Sck6_Pin|Sck7_Pin 
                          |Sck8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Dt4_Pin */
  GPIO_InitStruct.Pin = Dt4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Dt4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Dt3_Pin Dt2_Pin Dt1_Pin Dt5_Pin 
                           Dt6_Pin Dt7_Pin Dt8_Pin Lock2_Pin 
                           Lock1_Pin Door2_Pin */
  GPIO_InitStruct.Pin = Dt3_Pin|Dt2_Pin|Dt1_Pin|Dt5_Pin 
                          |Dt6_Pin|Dt7_Pin|Dt8_Pin|Lock2_Pin 
                          |Lock1_Pin|Door2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ButtonLed_Pin */
  GPIO_InitStruct.Pin = ButtonLed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ButtonLed_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Door1_Pin */
  GPIO_InitStruct.Pin = Door1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Door1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Relay_Pin */
  GPIO_InitStruct.Pin = Relay_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Relay_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
