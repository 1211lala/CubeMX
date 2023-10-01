#include ".\OLED\oled.h"

uint8_t OLED_RAM[128][4];	


/**
  * ��������: OLED����һ���ֽ�
  * �������: dat	���͵�����
  * �������: mode:����/�����־ OLED_CMD ��ʾ���� OLED_DATA ��ʾ����;
  * ˵    ��:	ʵ���Ƿ��������ֽ� �ֱ�λ  ������ַ  �����ַ�  ����
  */
void OLED_Write_Byte(uint8_t dat,uint8_t mode)
{
	iic_start();
	iic_send_byte(OLED_ARDESS);	/* д��ַ */
	iic_wait_ack();
	iic_send_byte(mode);				/* д�����ַ� */
	iic_wait_ack();
	iic_send_byte(dat);					/* д����*/
	iic_wait_ack();
	iic_stop();
}

/**
  * ��������: OLED��ʼ��
  * ˵    ��:	û�а���IIC���ŵĳ�ʼ������Ҫ����IIC��ʼ��֮��
  */
void OLED_Init(void)
{
	
	HAL_Delay(100);		/* OLED�ϵ��MCU�� */
	OLED_Write_Byte(0xAE,OLED_CMD); /*display off*/
	OLED_Write_Byte(0x00,OLED_CMD); /*set lower column address*/ 
	OLED_Write_Byte(0x10,OLED_CMD); /*set higher column address*/
	OLED_Write_Byte(0x00,OLED_CMD); /*set display start line*/ 
	OLED_Write_Byte(0xB0,OLED_CMD); /*set page address*/ 
	OLED_Write_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_Write_Byte(0xff,OLED_CMD); /*128*/ 
	OLED_Write_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_Write_Byte(0xA6,OLED_CMD); /*normal / reverse*/ 
	OLED_Write_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_Write_Byte(0x1F,OLED_CMD); /*duty = 1/32*/ 
	OLED_Write_Byte(0xC8,OLED_CMD); /*Com scan direction*/ 
	OLED_Write_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_Write_Byte(0x00,OLED_CMD); 
	OLED_Write_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_Write_Byte(0x80,OLED_CMD); 
	OLED_Write_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_Write_Byte(0x1f,OLED_CMD); 
	OLED_Write_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_Write_Byte(0x00,OLED_CMD); 
	OLED_Write_Byte(0xdb,OLED_CMD); /*set vcomh*/ 
	OLED_Write_Byte(0x40,OLED_CMD); 
	OLED_Write_Byte(0x8D,OLED_CMD); /*set charge pump enable*/ 
	OLED_Write_Byte(0x14,OLED_CMD);
	OLED_Clear(0);
	OLED_Write_Byte(0xAF,OLED_CMD); /*display ON*/ 
}

/**
  * ��������: ����OLED��ʾ 
  * ˵    ��: ��
  */
void OLED_DisPlay_On(void)
{
	OLED_Write_Byte(0x8D,OLED_CMD);	/* ��ɱ�ʹ�� */
	OLED_Write_Byte(0x14,OLED_CMD);	/* ������ɱ� */
	OLED_Write_Byte(0xAF,OLED_CMD);	/* ������Ļ */
}

/**
  * ��������: �ر�OLED��ʾ 
  * ˵    ��: ��
  */
void OLED_DisPlay_Off(void)
{
	OLED_Write_Byte(0x8D,OLED_CMD);/* ��ɱ�ʹ�� */
	OLED_Write_Byte(0x10,OLED_CMD);/* �رյ�ɱ� */
	OLED_Write_Byte(0xAE,OLED_CMD);/* �ر���Ļ */
}

/**
  * ��������: OLEDˢ��ʵ��д���ݺ���
  * �������: ��
  * �������: ��
  * ˵    ��:	��
  */
void OLED_Refresh(void)
{
	uint8_t i,n;
	for(i=0;i<4;i++)
	{
		OLED_Write_Byte(0xb0+i,OLED_CMD); /* ��������ʼ��ַ */
		OLED_Write_Byte(0x00,OLED_CMD);   /* ���õ�����ʼ��ַ ֻȡ0x00�ĵ���λ */
		OLED_Write_Byte(0x10,OLED_CMD);   /* ���ø�����ʼ��ַ ֻȡ0x10�ĵ���λ*/	
		iic_start();											/* �ϼ� 0x00 ������ʾ�е�ַ 0x00 */
		iic_send_byte(0x78);
		iic_wait_ack();
		iic_send_byte(0x40);
		iic_wait_ack();
		for(n=0;n<128;n++)
		{
			iic_send_byte(OLED_RAM[n][i]);
			iic_wait_ack();
		}
		iic_stop();
  }
}


/**
  * ��������: OLED��Ļ��������
	* �������: mode ȫ������ˢ��/ȫ��Ϩ��ˢ��  1:��  0:��
  * ˵    ��:	��
  */
void OLED_Clear(uint8_t mode)
{
	uint8_t dat;
	uint8_t i,n;
	
	if(mode){dat = 0xff;}
	else{dat = 0x00;}
	for(i=0;i<4;i++)
	{
	   for(n=0;n<128;n++)
			{
				OLED_RAM[n][i]=dat;
			}
  }
	OLED_Refresh(); /* ������ʾ */
}

/**
  * ��������: ���㺯��
	* �������: x �����x�� ��Χ:0-127
	* �������: y �����y�� ��Χ:0-31
	* �������: t ���/��� 1:��� 0:���	
  * ˵    ��:	��
  */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_RAM[x][i]|=n;}
	else
	{
		OLED_RAM[x][i]=~OLED_RAM[x][i];
		OLED_RAM[x][i]|=n;
		OLED_RAM[x][i]=~OLED_RAM[x][i];
	}
}

