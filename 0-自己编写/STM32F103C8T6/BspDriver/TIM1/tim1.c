#include "./TIM1/tim1.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim1;
DMA_HandleTypeDef tim1_dma;
uint16_t tim1_dma_ch2_buf[5] = {1000, 2000, 3000, 4000, 5000} ;

void MY_TIM1_Base_Tim_Config(uint16_t psc, uint16_t period)
{
	mytim1.Instance = TIM1;
	mytim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	mytim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	mytim1.Init.Period = period-1;
	mytim1.Init.Prescaler = psc-1;
	/* �ظ�������֪����ɶ�� */
	mytim1.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&mytim1);
	
	/* �����ʱ�����жϱ�־ */
	__HAL_TIM_CLEAR_FLAG(&mytim1,TIM_FLAG_UPDATE);
	/* ���жϷ�ʽ������ʱ�� */
	HAL_TIM_Base_Start_IT(&mytim1);
}




TIM_OC_InitTypeDef tim1_chx;
void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse)
{
	MY_TIM1_Base_Tim_Config(psc, period);
	HAL_TIM_PWM_Init(&mytim1);
	/* �����ʱ�����жϱ�־  TIM_Base_SetConfig()���������ô���һ�θ����ж� */
	__HAL_TIM_CLEAR_FLAG(&mytim1,TIM_FLAG_UPDATE);
	
	tim1_chx.OCFastMode = TIM_OCFAST_DISABLE;			
	tim1_chx.OCMode = TIM_OCMODE_PWM1;  					/* ����PWMģʽ */
	/* 
		ֻ�������ϼ���
		PWMMODE1	��CNT > CCR1ʱΪ��Ч��ƽ����֮Ϊ��Ч��ƽ
		PWMMODE2	��CNT > CCR1ʱΪ��Ч��ƽ����֮Ϊ��Ч��ƽ
	*/
	tim1_chx.OCPolarity =	TIM_OCPOLARITY_HIGH;
	tim1_chx.Pulse = pulse;							/* ���ñȽ�ֵ */
//	tim1_chx.OCIdleState =   	/* �߼���ʱ������ ���ÿ���״̬ */
//	tim1_chx.OCNIdleState =		/* �߼���ʱ������ ���û���ͨ������״̬ */								
//	tim1_chx.OCNPolarity =		/* �߼���ʱ������ ���û���ͨ�����м��� */
	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_chx, TIM_CHANNEL_1);
	/* ����PWM��� �ж�ģʽ */
	HAL_TIM_PWM_Start_IT(&mytim1, TIM_CHANNEL_1);
	
	
	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_chx, TIM_CHANNEL_2);
	tim1_dma.Instance = DMA1_Channel2;													/* ʹ�ù̶���DMAͨ�� */
	tim1_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;							/* DMA���� */
	tim1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;		/* DMA�����ڴ����ݴ�С */
	tim1_dma.Init.MemInc = DMA_MINC_ENABLE;											/* DMA�ڴ��ַ�Լ� */
	tim1_dma.Init.Mode = DMA_NORMAL;														/* DMA����ģʽ */
	tim1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;/* DMA����Ĵ������ݴ�С */
	tim1_dma.Init.PeriphInc = DMA_PINC_DISABLE;									/* DMA�Ĵ�����ַ���Լ� */
	tim1_dma.Init.Priority = DMA_PRIORITY_LOW;									/* DMA���ȼ��� */
	HAL_DMA_Init(&tim1_dma);																		/* ��ʼ��DMA���� */
	
	__HAL_LINKDMA(&mytim1, hdma[TIM_DMA_ID_CC2], tim1_dma);			/* �� DMA1_Channel2 �� CCR2 �Ĵ�����ϵ���� */
	
	
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	/* TIM1 �����ж�*/
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_UP_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
	
	if(htim == &mytim2)	/* TIM2 CH1 PA0 �ⲿ����� */
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM2_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	
	{
		__HAL_RCC_TIM1_CLK_ENABLE();		/* TIM1 CH1 PWM PA8  */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();			/* TIM1 CH1 PWM PA9  */
		
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};	
		
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		
		HAL_NVIC_SetPriority(TIM1_CC_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
		
																			
		
		
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn,3,0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	}
}






