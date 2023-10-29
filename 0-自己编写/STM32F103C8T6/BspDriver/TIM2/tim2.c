#include "./TIM2/tim2.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim2;

TIM_ClockConfigTypeDef tim2_clock;

/* ���ö�ʱ����ʱ����ԴΪ�ⲿ�ź� */
void MY_TIM2_CountMode_Config(uint16_t psc, uint16_t period)
{
	mytim2.Instance = TIM2;
	mytim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	mytim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	mytim2.Init.Period = period-1;
	mytim2.Init.Prescaler = psc-1;
	/* �ظ�������֪����ɶ�� */
	mytim2.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&mytim2);
	/* �����ʱ�����жϱ�־  TIM_Base_SetConfig()���������ô���һ�θ����ж� */
	__HAL_TIM_CLEAR_FLAG(&mytim2,TIM_FLAG_UPDATE);
	
	
	/*
		ѡ��ͬ��ʱ��Դʱ����Щ�������Բ������� ����ο� ���Ĳο��ֲ� 200ҳ
		psc���������Ƶ -- ע��
	*/
	/* �����˲����� ��ETR, ͨ��1��2��Ч �����Ĳο��ֲ�242ҳ�� IC1F λ*/
	tim2_clock.ClockFilter = 0x03;
	/* ���������źż��� ��ED˫������Ч */
	tim2_clock.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
	/* ���������Ƶ ֻ��ETRͨ������ */
	tim2_clock.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	/* �����ź���Դ */
	tim2_clock.ClockSource = TIM_TS_TI1FP1;		/* ʱ����ԴΪͨ��һ�ı�Ե���ģʽ */
	HAL_TIM_ConfigClockSource(&mytim2, &tim2_clock);
	
	
	/* ����TRIG�����ж� */
	__HAL_TIM_ENABLE_IT(&mytim2, TIM_IT_TRIGGER);
	/* ������ʱ�� �� ���������ж� */
	HAL_TIM_Base_Start_IT(&mytim2);
}


