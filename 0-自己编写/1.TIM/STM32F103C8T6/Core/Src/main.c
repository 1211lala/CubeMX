
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//#pragma import(__use_no_semihosting)
//void _sys_exit(int x)
//{
//  x = x;
//}
//struct __FILE
//{
//  int handle;
//};
//FILE __stdout;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t usb_buf[40];
uint8_t uart1_buf[40];
uint32_t count = 0;
uint32_t ccr1 = 0;
uint8_t flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
	
//	MY_TIM1_Chx_Pwm_Config(7200, 5000, 1000);
//	MY_TIM2_CountMode_Config(1,5);
//  MY_TIM3_Chx_IC_Config(72-1, 10000-1);	/* 10ms */
	

	my_uasrt1_uart_init(115200);
//	HAL_UART_Receive_IT(&myuart1, uart1_buf, 1);
	HAL_UARTEx_ReceiveToIdle_IT(&myuart1, uart1_buf, 40);
	MY_Key_Config();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_Delay(100);
		
		uint8_t key = Get_key_with_undo(30);
		if(key != key_null)
		{
			switch(key)
			{
				case(s1_down): break;
				case(s2_down): break;
				case(s3_down): break;
				case(s4_down): break;
			}
		}
		
		#if 0
		/* 上升沿触发*/
    if( flag == 0 && __HAL_TIM_GET_FLAG(&mytim3, TIM_FLAG_CC1) == SET)
    {
			count = 0;/* 清除上一次记录 */
			flag = 1;	/* 开始计时 */
			__HAL_TIM_CLEAR_FLAG(&mytim3, TIM_FLAG_CC1);
			ccr1 = TIM3->CCR1;
      sprintf((char*)usb_buf,"start\r\n");
      usb_transmit(usb_buf,strlen((char*)usb_buf));
			
			/* 下降沿触发*/
			tim3_chx.ICPolarity = TIM_ICPOLARITY_FALLING;
			HAL_TIM_IC_ConfigChannel(&mytim3, &tim3_chx, TIM_CHANNEL_1);
    }
		
		/* 下降沿触发*/
    if(flag == 1 && __HAL_TIM_GET_FLAG(&mytim3, TIM_FLAG_CC1) == SET)
    {
			flag = 0;	/* 停止计时 */
			__HAL_TIM_CLEAR_FLAG(&mytim3, TIM_FLAG_CC1);
			sprintf((char*)usb_buf,"stop\r\n");
      usb_transmit(usb_buf,strlen((char*)usb_buf));
			
			/* 设置上升沿触发*/
			tim3_chx.ICPolarity = TIM_ICPOLARITY_RISING;
			HAL_TIM_IC_ConfigChannel(&mytim3, &tim3_chx, TIM_CHANNEL_1);
    }
		#endif
    /* USER CODE END WHILE */
		
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* 定时器更新中断 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	if(htim->Instance == TIM1)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		usb_transmit((uint8_t*)"TIM1 UPDATE INTERRUPT\r\n", strlen((char*)"TIM1 UPDATE INTERRUPT\r\n"));
	}else if(htim->Instance == TIM2)
	{
		usb_transmit((uint8_t*)"TIM2 UPDATE INTERRUPT\r\n", strlen((char*)"TIM2 UPDATE INTERRUPT\r\n"));	
	}else if(htim->Instance == TIM3)
	{
		if(flag == 1)
		{
			float time = 0;
			
			if(count)
			{
				time = (10000 - ccr1 + (count)*10000 + TIM3->CNT) * 0.000001;
			}else
			{
				time = (10000 - ccr1) * 0.000001;
			}
			sprintf((char*)usb_buf, "down time %.4f \r\n", time);
			usb_transmit((uint8_t*)usb_buf, strlen((char*)usb_buf));
			count += 1;
		}
	}
}

/* 定时器触发中断 */
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{		
		

		sprintf((char*)usb_buf, "TIM2 TRIG INTERRUPT%d \r\n", TIM2->CNT);
		usb_transmit((uint8_t*)usb_buf, strlen((char*)usb_buf));
	}
}

/* 定时器PWM中断 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{									
		switch(htim->Channel)
		{
			case(HAL_TIM_ACTIVE_CHANNEL_1):
																			sprintf((char*)usb_buf, "TIM1 PWM CH1\r\n");
																			usb_transmit((uint8_t*)usb_buf, strlen((char*)usb_buf));
																			break;
			case(HAL_TIM_ACTIVE_CHANNEL_2): 
																			sprintf((char*)usb_buf, "TIM1 PWM CH2 INTERRUPT\r\n");
																			usb_transmit((uint8_t*)usb_buf, strlen((char*)usb_buf));
																			break;
			case(HAL_TIM_ACTIVE_CHANNEL_3): 
																			sprintf((char*)usb_buf, "TIM1 PWM CH3 DMA\r\n");
																			usb_transmit((uint8_t*)usb_buf, strlen((char*)usb_buf));break;
			case(HAL_TIM_ACTIVE_CHANNEL_4): break;
		}
	}
}

/* 串口一的接收中断回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &myuart1)
	{
		HAL_UART_Transmit(&myuart1, "int:", 4, 0xff);
		HAL_UART_Transmit(&myuart1, &uart1_buf[0], 1, 0xff);
		
		HAL_UART_Receive_IT(&myuart1, uart1_buf, 1);
	}
}


/* 串口一的IDLE事件回调函数 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &myuart1)
	{
		uart1_buf[Size] = '\0';
		
		HAL_UART_Transmit(&myuart1, "ent:", 4, 0xff);
		HAL_UART_Transmit(&myuart1, uart1_buf, Size, 0xff);
		
		HAL_UARTEx_ReceiveToIdle_IT(&myuart1, uart1_buf, 40);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
