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
#include "wwdg.h"
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
uint8_t iwdog_count = 8;
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
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_RNG_Init();
  MX_FSMC_Init();
  MX_TIM6_Init();
  
  /* USER CODE BEGIN 2 */
	printf("窗口看门狗的时钟来直PLCK1 = 42Mhz \r\n");
	printf("窗口看门狗的分频默认 /4096*设定的分频 本次设置8分频  42000000/4096/8 = 1281hz 左右\r\n\r\n");
	printf("窗口看门狗寄存器 WWDG_CR 是一个递减计数器 最高位(bit7)选择是否开启看门狗\r\n");
	printf("窗口看门狗寄存器 WWDG_CR bit6:bit0 保存当前计数值,当计数值小于0x40即等于0x3f时复位系统\r\n");
	printf("窗口看门狗寄存器 WWDG_CR 在开启中断后在计数值位0x40时会进入中断会留给我们一个计数值的时间来执行我们的操作\r\n");
	printf("窗口看门狗寄存器 WWDG_CR 本次设置的值为127\r\n\r\n");
	printf("窗口看门狗寄存器 WWDG_CFR 是用来保存我们设置的计数值设置\r\n");
	printf("窗口看门狗寄存器 WWDG_CFR(我们设置的窗口值)中的值 > WWDG_CR(实际的计数值)时才能喂狗\r\n");
	printf("窗口看门狗寄存器 WWDG_CFR(我们设置的窗口值)中的值 < WWDG_CR(实际的计数值)时喂狗也会引起系统护卫\r\n");
	printf("窗口看门狗寄存器 WWDG_CFR 本次设置的值为100\r\n\r\n");
	printf("本次窗口看门狗向下计数一的时间为 1/1281 = 0.0007806s \r\n");
	printf("本次窗口看门狗在计数值为127-100之间是不能喂狗的时间为 0.0007806s*27= 0.021077s \r\n");
	printf("本次窗口看门狗在计数值为100-64之间可以能喂狗时间为 0.021077s - 0.04995s之间\r\n\r\n");
	HAL_TIM_Base_Start_IT(&htim7);

	lcd_init(0, RED);
	g_back_color = RED;
	lcd_show_string(10, 10+24*0, 24, "5.1 Independent Watch Dog", BLACK);
	lcd_show_string(10, 10+24*2, 24, "use key to stop system restart", BLACK);
	
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, idle_rec_buf, 100);	/* 每次使用发生中断后不会自动恢复,要再次使用这个函数 */
	printf("system success\r\n");
	
		MX_WWDG_Init();					/* 要放在这个地方不能放在MX自动生成的地方不然个人初始化程序时间一长就会自动复位 */
	__HAL_WWDG_ENABLE_IT(&hwwdg,WWDG_IT_EWI);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_Delay(23);
		HAL_WWDG_Refresh(&hwwdg);
		HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
		
		HAL_Delay(47);
		HAL_WWDG_Refresh(&hwwdg);
		HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
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


void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
	printf("wwds timeout \r\n");
		
}
/* 定时器回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* 定时一秒 */
	if(htim == &htim7)
	{
		
	}
}

/* 串口IDLE中断函数 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &huart1)
	{
		idle_rec_buf[Size] = '\0';
		
		HAL_UART_Transmit(&huart1, idle_rec_buf, strlen((char*)idle_rec_buf), 0xff);
		
		HAL_UARTEx_ReceiveToIdle_IT(&huart1, idle_rec_buf, 100);	/* 再次打开串口IDLE中断接收函数 */
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
