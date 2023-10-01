/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include "can.h"
#include "rng.h"
#include "usart.h"
#include "gpio.h"

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
uint8_t uart1_rx_buf[100];
uint8_t uart1_rx_char = 0;
uint8_t uart1_rx_len  = 0;

uint8_t uart2_rx_buf[10];			/* ���ܸ�̫�� */
uint8_t uart2_rx_char = 0;
uint8_t uart2_rx_len  = 0;
uint8_t flag_485 = 0;


uint8_t can_send[8];
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
  MX_CAN1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
	/*����IDLE�ж�*/
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	/*�������ڽ����ж�*/
  __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
	/*���ý����ַ�,����*/
  HAL_UART_Receive_IT(&huart1, &uart1_rx_char, 1);
	
	
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
	/*�������ڽ����ж�*/
  __HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
	/*���ý����ַ�,����*/
  HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);
	
	/* RS485�ϵ�Ĭ��Ϊ����ģʽ */
	HAL_GPIO_WritePin(RE_485_GPIO_Port, RE_485_Pin, GPIO_PIN_RESET);
	
	
	/* CAN���� */
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);
	CAN1_Set_Filter();

 /* USER CODE BEGIN 2 */
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t ID = 1;
  while (1)
  {
		uint8_t keystatus = Get_Keys_Status();
		if(keystatus)
		{
			if(keystatus == KEY2)						/* ��������֡ */
			{
				can_send[0] = RNG_GetRandomNumber(0, 255);
				can_send[1] = RNG_GetRandomNumber(0, 255);
				can_send[2] = RNG_GetRandomNumber(0, 255);
				can_send[3] = RNG_GetRandomNumber(0, 255);
				can_send[4]	= RNG_GetRandomNumber(0, 255);
				can_send[5] = RNG_GetRandomNumber(0, 255);
				can_send[6] = RNG_GetRandomNumber(0, 255);
				can_send[7] = RNG_GetRandomNumber(0, 255);
				printf("���͵���ϢΪ\r\n");
				printf("can_send[0] = %d\r\n",can_send[0]);
				printf("can_send[1] = %d\r\n",can_send[1]);
				printf("can_send[2] = %d\r\n",can_send[2]);
				printf("can_send[3] = %d\r\n",can_send[3]);
				printf("can_send[4] = %d\r\n",can_send[4]);
				printf("can_send[5] = %d\r\n",can_send[5]);
				printf("can_send[6] = %d\r\n",can_send[6]);
				printf("can_send[7] = %d\r\n",can_send[7]);
				CAN1_Send_Data_Message(ID++, can_send, sizeof can_send);

				CAN1_Receive_Message();
			}
			else if(keystatus == KEY3)			/* ����ң��֡ */
			{
				CAN1_Send_Remote_Message(ID++);
				CAN1_Receive_Message();
			}
		}
		
		
		
		static uint8_t led_count;
		led_count++;
		HAL_Delay(1);
		if(led_count > 250)
		{
			led_count = 0;
			HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
			printf("RNG--%d",RNG_GetRandomNumber(1, 8));
		}
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
  RCC_OscInitStruct.PLL.PLLQ = 14;
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

/* ����IDLE�жϻص� */
void UART_IDLECallBack(UART_HandleTypeDef *huart)
{
	if(huart ->Instance == USART1)
	{
    /*��ӽ��շ�*/
    uart1_rx_buf[uart1_rx_len] = '\0';
    /*���ڷ��ͽ��յ�����Ϣ*/
    HAL_UART_Transmit(&huart1, uart1_rx_buf, strlen((char*)uart1_rx_buf),100);
    /*������һ�ν���*/
    uart1_rx_len=0;
    /***********������***********/
		
	}
	
	if(huart ->Instance == USART2)
	{
    /*��ӽ��շ�*/
    uart2_rx_buf[uart2_rx_len] = '\0';
    /*���ڷ��ͽ��յ�����Ϣ*/
		RS485_Send_Meg(uart2_rx_buf, strlen((char*)uart2_rx_buf));	
    uart2_rx_len=0;
    /***********������***********/

	}
	
}

/* �����жϻص����� */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
    /*���յ�ǰ������*/
    uart1_rx_buf[uart1_rx_len] = uart1_rx_char;
    uart1_rx_len++;
    /*�ٴδ򿪴����ж�*/
    HAL_UART_Receive_IT(&huart1, &uart1_rx_char, 1);
  }
	
	if(huart ->Instance == USART2)
	{
		/*���յ�ǰ������*/
    uart2_rx_buf[uart2_rx_len] = uart2_rx_char;
    uart2_rx_len++;
    /*�ٴδ򿪴����ж�*/
    HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);
	}
	
}

/* ���ڷ�����ɻص����� */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
		
  }
}


/* CAN��FIFO0���ջص����� */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == &hcan1)
	{
		uint8_t can_receive[8];
		
		CAN_RxHeaderTypeDef 	CAN1_RxHeader;
		
		if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &CAN1_RxHeader, can_receive) != HAL_OK)
		{
			printf("HAL_CAN_RxFifo0MsgPendingCallback ����ʧ��\r\n");
		}
		else
		{
			printf("StdId = %d\r\n",CAN1_RxHeader.StdId);
			printf("IDE (��׼��ʽ:0 ��չ��ʽ:4)\t = %d\r\n",CAN1_RxHeader.IDE);
			printf("RTR (����֡  :0 ң��֡  :2)\t = %d\r\n",CAN1_RxHeader.RTR);
			if(CAN1_RxHeader.RTR == CAN_RTR_DATA)				/* �ж��Ƿ�Ϊ����֡ */
			{
				printf("DLC (���ݳ���)\t\t\t = %d\r\n",CAN1_RxHeader.DLC);
				printf("can_receive[0] = %d\r\n",can_receive[0]);
				printf("can_receive[1] = %d\r\n",can_receive[1]);
				printf("can_receive[2] = %d\r\n",can_receive[2]);
				printf("can_receive[3] = %d\r\n",can_receive[3]);
				printf("can_receive[4] = %d\r\n",can_receive[4]);
				printf("can_receive[5] = %d\r\n",can_receive[5]);
				printf("can_receive[6] = %d\r\n",can_receive[6]);
				printf("can_receive[7] = %d\r\n",can_receive[7]);
			}
			printf("\r\n\r\n\r\n");
		}
		
	}
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == &hcan1)
	{
		uint8_t can_receive[8];
		
		CAN_RxHeaderTypeDef 	CAN1_RxHeader;
		
		if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO1, &CAN1_RxHeader, can_receive) != HAL_OK)
		{
			printf("HAL_CAN_RxFifo0MsgPendingCallback ����ʧ��\r\n");
		}
		else
		{
			printf("StdId = %d\r\n",CAN1_RxHeader.StdId);
			printf("IDE (��׼��ʽ:0 ��չ��ʽ:4)\t = %d\r\n",CAN1_RxHeader.IDE);
			printf("RTR (����֡  :0 ң��֡  :2)\t = %d\r\n",CAN1_RxHeader.RTR);
			if(CAN1_RxHeader.RTR == CAN_RTR_DATA)				/* �ж��Ƿ�Ϊ����֡ */
			{
				printf("DLC (���ݳ���)\t\t\t = %d\r\n",CAN1_RxHeader.DLC);
				printf("can_receive[0] = %d\r\n",can_receive[0]);
				printf("can_receive[1] = %d\r\n",can_receive[1]);
				printf("can_receive[2] = %d\r\n",can_receive[2]);
				printf("can_receive[3] = %d\r\n",can_receive[3]);
				printf("can_receive[4] = %d\r\n",can_receive[4]);
				printf("can_receive[5] = %d\r\n",can_receive[5]);
				printf("can_receive[6] = %d\r\n",can_receive[6]);
				printf("can_receive[7] = %d\r\n",can_receive[7]);
			}
			printf("\r\n\r\n\r\n");
		}
		
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
