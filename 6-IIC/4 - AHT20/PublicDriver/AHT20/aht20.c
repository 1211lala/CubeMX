#include "./AHT20/aht20.h"


/**
 * @brief   д��IICд��ַ��һ���ֽ�����
 * @param		data : д�������
 * @retval  ��
 */
void AHT20_Write_Byte(uint8_t data)
{
	iic1_start();
	iic1_send_byte(W_ADDRESS);
	iic1_wait_ack();
	iic1_send_byte(data);
	iic1_wait_ack();
	iic1_stop();
}

/**
 * @brief   д��IICд��ַ+ָ��+��������
 * @param		cmd  : д���ָ��
 * @param		data : д�������
 * @param		data : ���ݳ���
 * @retval  ��
 */
void AHT20_Write_Bytes(uint8_t cmd, uint8_t * data, uint16_t len)
{
	iic1_start();
	iic1_send_byte(W_ADDRESS);
	iic1_wait_ack();
	iic1_send_byte(cmd);
	iic1_wait_ack();
	for(uint16_t i=0; i<len; i++)
	{
		iic1_send_byte(*(data+i));
		iic1_wait_ack();
	}
	iic1_stop();
}

/**
 * @brief   ���AHT20��״̬�ֽ�
 * @retval  ״̬�ֽ�
 */
uint8_t AHT20_Get_Status_Byte(void)
{
	iic1_start();
	iic1_send_byte(0x71);
	iic1_wait_ack();
	uint8_t rec = iic1_read_byte(0);
	iic1_stop();	
	return rec;
}

/**
 * @brief   ���AHT20��״̬�ֽڵ�bit3λ
 * @retval  1:bit3 = 1���Զ�ȡ / 0��bit3 = 0 �����Զ�ȡ�跢�ͳ�ʼ������
 */
uint8_t AHT20_Get_Cal_Enable(void)
{
	uint8_t status = AHT20_Get_Status_Byte();
	if((status & 0x08) == 0x08)			/* �ٷ�����������0x68���룬����ֻ�ж� bit3 Cal_Enableλ */
	{
		return 1;	/* ���Զ�ȡ */
	}
	return 0;		/* �����Զ�ȡ */
}


/**
 * @brief   ��AHT20���г�ʼ����
 * @retval  1:bit3 = 1���Զ�ȡ / 0��bit3 = 0 �����Զ�ȡ�跢�ͳ�ʼ������
 * @note  ��ΪAHT20���ϵ��MCU��Ҫ�ڴ˺���ǰ��ʱ50ms����
 * @note  ע��˺���û�ж�IICӲ���ӿڳ�ʼ��������ʹ���������ǰʹ��iic1_init()����
 */
uint8_t AHT20_Init(void)
{
	uint8_t status = AHT20_Get_Cal_Enable();
	if(status){return 1;}
	else
	{
		uint8_t data[2] = {0x08, 0x00};
		AHT20_Write_Bytes(INIT_CMD, data, 2);
		HAL_Delay(10);
		status = AHT20_Get_Cal_Enable();
		if(status){return 1;}
	}
	return 0;
}

/**
 * @brief ����AHT20�������� 
 * @note  ��������������Ҫ�ȴ�75ms�����ܶ�ȡAHT20��ֵ
 */
void AHT20_Start_Measure(void)
{
	uint8_t data[2] = {0x33, 0x00};
	AHT20_Write_Bytes(START_CMD, data, 2);
}

/**
 * @brief   ��ȡATH20����ʪ��ԭʼ����
 * @param		buf  : �������ݵĵ�ַ
 * @retval  ��
 */
uint8_t AHT20_Read_Hex_Hum_Temp(uint32_t* buf)
{
	uint32_t hum_temp = 0;
	
	uint8_t rec = AHT20_Get_Status_Byte();
	
	while(rec & 0x80 )	/* �ȴ�ת�����, ����ȴ�100ms��û���ݾ��˳����� */
	{
		
	}
	
	iic1_start();
	iic1_send_byte(0x71);
	iic1_wait_ack();
	
	uint8_t byte_1th = iic1_read_byte(1);	/* ״̬�ֽڲ���Ҫ���� */
	uint8_t byte_2th = iic1_read_byte(1);	/* ʪ������ */
	uint8_t byte_3th = iic1_read_byte(1);	/* ʪ������ */
	uint8_t byte_4th = iic1_read_byte(1);	/* ���°�ʪ*/
	uint8_t byte_5th = iic1_read_byte(1);	/* �¶����� */
	uint8_t byte_6th = iic1_read_byte(0);	/* �¶����� */
	
	hum_temp = (hum_temp|byte_2th)<<8;
	hum_temp = (hum_temp|byte_3th)<<8;
	hum_temp = (hum_temp|byte_4th);
	hum_temp =hum_temp >>4;
	buf[0] = hum_temp;   		//ʪ��
	hum_temp = 0;
	hum_temp = (hum_temp|byte_4th)<<8;
	hum_temp = (hum_temp|byte_5th)<<8;
	hum_temp = (hum_temp|byte_6th);
	hum_temp = hum_temp&0xfffff;
	buf[1] =hum_temp; 			//�¶�
	
	return 1;
}



/**
 * @brief   ��ȡATH20��Float���� 
 * @param		buf :�������ݵĵ�ַ buf[0] = ʪ��  buf[1] = �¶�
 * @retval  ��
 */
uint8_t AHT20_Read_Float_Hum_Temp(float *buf)
{
	uint32_t hex_data[2];
	
	
	AHT20_Init();
	AHT20_Start_Measure();
	
	HAL_Delay(75);
	uint8_t status = AHT20_R ead_Hex_Hum_Temp(hex_data);

	
	buf[0] = (hex_data[0]*100*10/1024/1024);  
	buf[1] = (hex_data[1]*200*10/1024/1024-500);
	
	return 1;
}

