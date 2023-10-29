#include "./TIM1/tim1.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim1;


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




TIM_OC_InitTypeDef tim1_ch1;
void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse)
{
	MY_TIM1_Base_Tim_Config(psc, period);
	HAL_TIM_PWM_Init(&mytim1);
	/* �����ʱ�����жϱ�־  TIM_Base_SetConfig()���������ô���һ�θ����ж� */
	__HAL_TIM_CLEAR_FLAG(&mytim1,TIM_FLAG_UPDATE);
	
	tim1_ch1.OCFastMode = TIM_OCFAST_DISABLE;			
	tim1_ch1.OCMode = TIM_OCMODE_PWM1;  					/* ����PWMģʽ */
	/* 
		ֻ�������ϼ���
		PWMMODE1	��CNT > CCR1ʱΪ��Ч��ƽ����֮Ϊ��Ч��ƽ
		PWMMODE2	��CNT > CCR1ʱΪ��Ч��ƽ����֮Ϊ��Ч��ƽ
	*/
	tim1_ch1.OCPolarity =	TIM_OCPOLARITY_HIGH;
	tim1_ch1.Pulse = pulse;							/* ���ñȽ�ֵ */
//	tim1_ch1.OCIdleState =   	/* �߼���ʱ������ ���ÿ���״̬ */
//	tim1_ch1.OCNIdleState =		/* �߼���ʱ������ ���û���ͨ������״̬ */								
//	tim1_ch1.OCNPolarity =		/* �߼���ʱ������ ���û���ͨ�����м��� */
	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_ch1, TIM_CHANNEL_1);
	
	/* ����PWM��� */
	HAL_TIM_PWM_Start(&mytim1, TIM_CHANNEL_1);

	
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	/* TIM1 �����ж�*/
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_UP_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
	
	if(htim == &mytim2)	/* TIM2 �ⲿ����� */
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM2_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	/* TIM1 PWM */
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}






