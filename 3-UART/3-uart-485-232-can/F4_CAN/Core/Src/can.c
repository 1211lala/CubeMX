/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 5;
  hcan1.Init.Mode = CAN_MODE_LOOPBACK;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_7TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* ����ɸѡ���͹���FIFO */
HAL_StatusTypeDef CAN1_Set_Filter(void)
{
	CAN_FilterTypeDef CAN1_Filter;
	
	/* ����32λ��ʶ������ģʽ */
	CAN1_Filter.FilterBank = 0;
	CAN1_Filter.FilterMode = CAN_FILTERMODE_IDMASK;								/* ����ΪID������ģʽ */
	CAN1_Filter.FilterScale = CAN_FILTERSCALE_32BIT;							/* ����32λ */
	
	/* ����ɸѡ�� */
	CAN1_Filter.FilterIdHigh = 0x0020;
	CAN1_Filter.FilterIdLow =	0x0000;
	CAN1_Filter.FilterMaskIdHigh = 0x0020;
	CAN1_Filter.FilterMaskIdLow =	0x0000;
	
	CAN1_Filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;					/* ������FIFO0 */
	CAN1_Filter.FilterActivation =	CAN_FILTER_ENABLE;						/* ʹ��ɸѡ�� */
	CAN1_Filter.SlaveStartFilterBank = 14; 												/* CAN1ʹ��0-14ɸѡ�� CAN2ʹ��14-27 */
	
  HAL_StatusTypeDef result = HAL_CAN_ConfigFilter(&hcan1, &CAN1_Filter);
	return result;
}



/* CAN�������ͺ��� */
void CAN1_Send_Data_Message(uint8_t megID, uint8_t *send_buf, uint8_t len)
{
	CAN_TxHeaderTypeDef CAN1_TxHeader;
	
	CAN1_TxHeader.StdId = megID;													/* ���ñ�׼֡ID*/
//	CAN1_TxHeader.ExtId =																	/* ������չ֡ID*/
	
	CAN1_TxHeader.IDE =	CAN_ID_STD;												/* ������չ��ʽ���Ǳ�׼��ʽ*/
	CAN1_TxHeader.RTR = CAN_RTR_DATA;												/* ����Ϊ����֡����ң��֡ */
	CAN1_TxHeader.DLC = len;																/* ���÷��������ֽڴ�С */
	CAN1_TxHeader.TransmitGlobalTime = DISABLE;						/* ���ò�����ʱ���*/
	
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) <1 );		/* �鿴���޿��õ�����*/
	
	uint32_t TxMailbox;																		/* ����ʹ�õķ������� */
	if(HAL_CAN_AddTxMessage(&hcan1, &CAN1_TxHeader, send_buf, &TxMailbox) != HAL_OK)
	{
		printf("HAL_CAN_AddTxMessage ����ʧ��\r\n");
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3);	/* �ȴ�������� */
}


/* CAN�������ͺ��� */
void CAN1_Send_Remote_Message(uint8_t megID)
{
	CAN_TxHeaderTypeDef CAN1_TxHeader;
	
	CAN1_TxHeader.StdId = megID;													/* ���ñ�׼֡ID*/
//	CAN1_TxHeader.ExtId =																/* ������չ֡ID*/
	
	CAN1_TxHeader.IDE =	CAN_ID_STD;												/* ������չ��ʽ���Ǳ�׼��ʽ*/
	CAN1_TxHeader.RTR = CAN_RTR_REMOTE;										/* ����Ϊ����֡����ң��֡ */
	CAN1_TxHeader.DLC = 2;																/* ���÷��������ֽڴ�С */
	CAN1_TxHeader.TransmitGlobalTime = DISABLE;						/* ���ò�����ʱ���*/
	
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) <1 );		/* �鿴���޿��õ�����*/
	
	uint8_t send_buf[8];																	/* ң��֡��������Ϣ */
	uint32_t TxMailbox;																		/* ����ʹ�õķ������� */
	if(HAL_CAN_AddTxMessage(&hcan1, &CAN1_TxHeader, send_buf, &TxMailbox) != HAL_OK)
	{
		printf("HAL_CAN_AddTxMessage ����ʧ��\r\n");
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3);	/* �ȴ�������� */
}


/* CAN�������պ��� */
void CAN1_Receive_Message(void)
{
	if(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) != 1)
	{
		/* ����������û����Ϣ�˳����� */
		return ;
	}
	
	CAN_RxHeaderTypeDef CAN1_RxHeader;							/* ���վ�� */
	
	uint8_t receive_data[8];												/* ���ݽ��ջ��� */
	
	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &CAN1_RxHeader, receive_data) != HAL_OK)
	{
		printf("HAL_CAN_GetRxMessage ����ʧ��\r\n");
	}
	else
	{
		printf("StdId = %d\r\n",CAN1_RxHeader.StdId);
		printf("IDE (��׼��ʽ:0 ��չ��ʽ:4)\t = %d\r\n",CAN1_RxHeader.IDE);
		printf("RTR (����֡  :0 ң��֡  :2)\t = %d\r\n",CAN1_RxHeader.RTR);
		if(CAN1_RxHeader.RTR == CAN_RTR_DATA)				/* �ж��Ƿ�Ϊ����֡ */
		{
			printf("DLC (���ݳ���)\t\t\t = %d\r\n",CAN1_RxHeader.DLC);
			printf("receive_data[0] = %d\r\n",receive_data[0]);
			printf("receive_data[1] = %d\r\n",receive_data[1]);
			printf("receive_data[2] = %d\r\n",receive_data[2]);
			printf("receive_data[3] = %d\r\n",receive_data[3]);
			printf("receive_data[4] = %d\r\n",receive_data[4]);
			printf("receive_data[5] = %d\r\n",receive_data[5]);
			printf("receive_data[6] = %d\r\n",receive_data[6]);
			printf("receive_data[7] = %d\r\n",receive_data[7]);
		}
		printf("\r\n\r\n\r\n");
	
	}
}
/* USER CODE END 1 */