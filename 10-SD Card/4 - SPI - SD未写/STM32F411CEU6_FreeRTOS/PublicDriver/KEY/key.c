#include "./KEY/key.h"




/**
 * ��������: ���»�ȡ��ֵ
 * �������: ����ʱ��
 * �� �� ֵ: ���µļ�ֵ
 * ˵    ��: ������
 */
uint8_t Get_key_with_down(uint8_t waittime)
{
	/* ����uwtick��ֵ */
	static uint32_t tick;
	/* ������־λ */
	static uint8_t flag = 0;
	uint8_t key_s1_status,key_s2_status,key_s3_status,key_s4_status;
	
	key_s1_status = HAL_GPIO_ReadPin(key_s1_GPIO_Port, key_s1_Pin);
	key_s2_status = HAL_GPIO_ReadPin(key_s2_GPIO_Port, key_s2_Pin);
	key_s3_status = HAL_GPIO_ReadPin(key_s3_GPIO_Port, key_s3_Pin);
	key_s4_status = HAL_GPIO_ReadPin(key_s4_GPIO_Port, key_s4_Pin);
	
	/* ����а������� ���� if ���ֵ�ǰ uwtick��ֵ */
	if(flag == 0 && (key_s1_status == DOWN_STATUS || key_s2_status == DOWN_STATUS || key_s3_status == DOWN_STATUS || key_s4_status == DOWN_STATUS))
	{
		flag = 1;
		tick = uwTick;
		
	}
	/* ��һ���а������º� �ȴ� waittimeʱ�� �����������if����İ������, ˵��ȷʵ�а������·��ر�� */
	if(flag == 1 &&  (uwTick - tick ) > waittime)
	{
		if(flag == 1 && key_s1_status == DOWN_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			return s1_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == DOWN_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			return s2_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == DOWN_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			return s3_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == DOWN_STATUS)
		{
			flag = 2;
			return s4_down;
		}
	}
	/* ��û�а�������ʱ��flag =0 �ȴ��������� */
	if(key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
	{
		flag = 0;
	}
	return key_null;
}



/**
 * ��������: �ɿ���ȡ��ֵ
 * �������: ����ʱ��
 * �� �� ֵ: ���µļ�ֵ
 * ˵    ��: ������
 */
uint8_t Get_key_with_undo(uint8_t waittime)
{
	/* ����uwtick��ֵ */
	static uint32_t tick;
	/* ������־λ */
	static uint8_t flag = 0;
	/* �����Ǹ��������� */
	static uint8_t down = 0;
	
	uint8_t key_s1_status,key_s2_status,key_s3_status,key_s4_status;
	key_s1_status = HAL_GPIO_ReadPin(key_s1_GPIO_Port, key_s1_Pin);
	key_s2_status = HAL_GPIO_ReadPin(key_s2_GPIO_Port, key_s2_Pin);
	key_s3_status = HAL_GPIO_ReadPin(key_s3_GPIO_Port, key_s3_Pin);
	key_s4_status = HAL_GPIO_ReadPin(key_s4_GPIO_Port, key_s4_Pin);
	/* ����а������� ���� if ���ֵ�ǰ uwtick��ֵ */
	if(flag == 0 && (key_s1_status == DOWN_STATUS || key_s2_status == DOWN_STATUS || key_s3_status == DOWN_STATUS || key_s4_status == DOWN_STATUS))
	{
		flag = 1;
		tick = uwTick;
	}
	/* ��һ���а������º� �ȴ� waittimeʱ�� �����������if����İ������, ˵��ȷʵ�а������·��ر�� */
	if(flag == 1 &&  (uwTick - tick ) > waittime)
	{
		if(flag == 1 && key_s1_status == DOWN_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			down = s1_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == DOWN_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			down = s2_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == DOWN_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			down = s3_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == DOWN_STATUS)
		{
			flag = 2;
			down = s4_down;
		}
	}
	/* ��û�а�������ʱ��flag =0 �ȴ��������� */
	if(flag == 2 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
	{
		flag = 0;
		return down;
	}
	return key_null;
}



