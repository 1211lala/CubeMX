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
#include "rng.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint8_t idle_rec_buf[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* ��֪����������C������ʹ�ð������ĺ��� */
#pragma import(__use_no_semihosting)
 
/* ���� _sys_exit() �Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
	x = x;
}
 
/* ��׼����Ҫ��֧������ */
struct __FILE
{
	int handle;
};
 
FILE __stdout;
	
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
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_RNG_Init();
  MX_FSMC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	printf("������:ʹ��FSMC�Ŀ�1(BANK1)������SRAM, ��1��ַ��Χ�ܴ�СΪ256MB,���ֳ�4��\r\n");
	printf("�洢��1(FSMC_NE1)��ַ��Χ: 0X6000 0000 ~ 0X63FF FFFF\r\n");
	printf("�洢��2(FSMC_NE2)��ַ��Χ: 0X6400 0000 ~ 0X67FF FFFF\r\n");
	printf("�洢��3(FSMC_NE3)��ַ��Χ: 0X6800 0000 ~ 0X6BFF FFFF\r\n");
	printf("�洢��4(FSMC_NE4)��ַ��Χ: 0X6C00 0000 ~ 0X6FFF FFFF\r\n");
	
	my_mem_init(SRAMEX);	/* ���SRAM��MALLOC���� */
	
	uint8_t *wrt = NULL;
	uint8_t *dis = NULL;
	
	
	lcd_init(0, WHITE);
	lcd_show_string(10, 0+24*0, 24, "4.2 SRAM MALLOC", RED);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, idle_rec_buf, 100);	/* ÿ��ʹ�÷����жϺ󲻻��Զ��ָ�,Ҫ�ٴ�ʹ��������� */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		uint8_t key = Get_key_with_undo(20);
		if(key == key_up_down)
		{
			wrt = mymalloc(SRAMEX, 2*1024);	/* ����ַ�����ڴ� */
			if(wrt != NULL)
			{
				sprintf((char*)wrt, "hello worldqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqppp\
														 helloworldqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqppp\
														 helloworldqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqppp\
														 qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq");
				printf("%s\r\n", wrt);
			}
			
		}
		
		if(key == key0_down)
		{
			myfree(SRAMEX, wrt);	/* �ͷŷ�����ڴ� */
		}
		
		dis = mymalloc(SRAMEX, 1*1024);		/* ����ַ�����ڴ� */
		uint16_t memused = my_mem_perused(SRAMEX);
		sprintf((char *)dis, " SRAMEX(963Kb) used %d.%01d%%", memused / 10, memused % 10);  
		lcd_show_string(10, 0+24*2, 32, (char*)dis, RED);
		myfree(SRAMEX,dis);	/* �ͷŷ�����ڴ� */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* ��ʱ���ص����� */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* ��ʱһ�� */
	if(htim == &htim7)
	{
		HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
	}
}

/* ����IDLE�жϺ��� */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &huart1)
	{
		idle_rec_buf[Size] = '\0';
		
		HAL_UART_Transmit(&huart1, idle_rec_buf, strlen((char*)idle_rec_buf), 0xff);
		
		HAL_UARTEx_ReceiveToIdle_IT(&huart1, idle_rec_buf, 100);	/* �ٴδ򿪴���IDLE�жϽ��պ��� */
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
