#include "./OLED/oled.h"
#include "./OLED/oled_font.h"
uint8_t OLED_RAM[128][4];

/**
 * ��������: OLED����һ���ֽ�
 * �������: dat	���͵�����
 * �������: mode:����/�����־ OLED_CMD ��ʾ���� OLED_DATA ��ʾ����;
 * ˵    ��:	ʵ���Ƿ��������ֽ� �ֱ�λ  ������ַ  �����ַ�  ����
 */
void OLED_Write_Byte(uint8_t dat, uint8_t mode)
{
	iic_start();
	iic_send_byte(OLED_ARDESS); /* д��ַ */
	iic_wait_ack();
	iic_send_byte(mode); /* д�����ַ� */
	iic_wait_ack();
	iic_send_byte(dat); /* д����*/
	iic_wait_ack();
	iic_stop();
}

/**
 * ��������: OLED��ʼ��
 * ˵    ��:	û�а���IIC���ŵĳ�ʼ������Ҫ����IIC��ʼ��֮��
 */
void OLED_Init(void)
{

	HAL_Delay(100);					 /* OLED�ϵ��MCU�� */
	OLED_Write_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_Write_Byte(0x00, OLED_CMD); /*set lower column address*/
	OLED_Write_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_Write_Byte(0x00, OLED_CMD); /*set display start line*/
	OLED_Write_Byte(0xB0, OLED_CMD); /*set page address*/
	OLED_Write_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_Write_Byte(0xff, OLED_CMD); /*128*/
	OLED_Write_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_Write_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_Write_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_Write_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
	OLED_Write_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_Write_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_Write_Byte(0x00, OLED_CMD);
	OLED_Write_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_Write_Byte(0x80, OLED_CMD);
	OLED_Write_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_Write_Byte(0x1f, OLED_CMD);
	OLED_Write_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_Write_Byte(0x00, OLED_CMD);
	OLED_Write_Byte(0xdb, OLED_CMD); /*set vcomh*/
	OLED_Write_Byte(0x40, OLED_CMD);
	OLED_Write_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
	OLED_Write_Byte(0x14, OLED_CMD);
	OLED_Clear(0);
	OLED_Write_Byte(0xAF, OLED_CMD); /*display ON*/
}

/**
 * ��������: ����OLED��ʾ
 * ˵    ��: ��
 */
void OLED_DisPlay_On(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD); /* ��ɱ�ʹ�� */
	OLED_Write_Byte(0x14, OLED_CMD); /* ������ɱ� */
	OLED_Write_Byte(0xAF, OLED_CMD); /* ������Ļ */
}

/**
 * ��������: �ر�OLED��ʾ
 * ˵    ��: ��
 */
void OLED_DisPlay_Off(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD); /* ��ɱ�ʹ�� */
	OLED_Write_Byte(0x10, OLED_CMD); /* �رյ�ɱ� */
	OLED_Write_Byte(0xAE, OLED_CMD); /* �ر���Ļ */
}

/**
 * ��������: OLEDˢ��ʵ��д���ݺ���
 * �������: ��
 * �������: ��
 * ˵    ��:	��
 */
void OLED_Refresh(void)
{
	uint8_t i, n;
	for (i = 0; i < 4; i++)
	{
		OLED_Write_Byte(0xb0 + i, OLED_CMD); 	/* ��������ʼ��ַ */
		OLED_Write_Byte(0x00, OLED_CMD);	 		/* ���õ�����ʼ��ַ ֻȡ0x00�ĵ���λ */
		OLED_Write_Byte(0x10, OLED_CMD);	 		/* ���ø�����ʼ��ַ ֻȡ0x10�ĵ���λ*/
		iic_start();						 							/* �ϼ� 0x00 ������ʼ�е�ַ 0x00 */
		iic_send_byte(0x78);
		iic_wait_ack();
		iic_send_byte(0x40);
		iic_wait_ack();
		for (n = 0; n < 128; n++)
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
	uint8_t i, j;

	if (mode)
	{
		dat = 0xff;
	}
	else
	{
		dat = 0x00;
	}
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 128; j++)
		{
			OLED_RAM[j][i] = dat;
		}
	}
}

/**
 * ��������: ���㺯��
 * �������: x �����x�� ��Χ:0-127
 * �������: y �����y�� ��Χ:0-31
 * �������: t ���/��� 1:��� 0:���
 * ˵    ��:	��
 */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
	uint8_t i, j;
	i = y / 8; /* ��������ڵڼ��� */
	j = y % 8; /* ��������ڵڼ��еľ�����һλ*/
	if (t)
	{
		OLED_RAM[x][i] |= 1 << j;
	}
	else
	{
		OLED_RAM[x][i] &= ~(1 << j);
	}
}

/**
 * ��������: ��ʾһ��Ӣ���ַ�
 * �������: x �����x�� ��Χ:0-127
 * �������: y �����y�� ��Χ:0-31
 * �������: chr ��Ҫ��ʾ���ַ�
 * �������: char_size ��ʾ�ַ��Ĵ�С
 * �������: mode ������ʾ/��ɫ��ʾ 1 ������ʾ 0 ��ɫ��ʾ
 * ˵    ��: ���� 8*6������
 */
/*
1. ----->		2. ------->
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
3. ----->		4. ------->
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
*/
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size, uint8_t mode)
{
	uint8_t ram_size, size1, chr1, array, x0, y0;
	x0 = x, y0 = y;

	ram_size = char_size / 8;
	if (char_size % 8)
	{
		ram_size += 1;
	}
	ram_size = ram_size * (char_size / 2); /* ���/2���ǵó��������Ŀ���Ƕ��� ��12*6 32*16, ���Ҫ�ĳɳ��Ϳ�һ���������Ҫ�ĳ� /1 ͬ���Ƴ����������� */

	if(char_size == 8) ram_size = 6;	/* Ϊ�˼��� 8*6 ������ */

	chr1 = chr - ' '; /* ����ƫ�ƺ��ֵ */

	for (uint8_t i = 0; i < ram_size; i++)
	{
		switch (char_size)
		{
			case(8) : array = ascll_0806[chr1][i];break;
			case(16): array = ascll_1608[chr1][i]; break;
			case(24): array = ascll_2412[chr1][i];break;
			case(32):break;
		}

		for (uint8_t j = 0; j < 8; j++) /* д��һ���ֽڵ�����*/
		{
			if (array & 0x01) /* ��λ��ȡ�ֽ�����*/
			{
				OLED_DrawPoint(x, y, mode);
			}
			else
			{
				OLED_DrawPoint(x, y, !mode);
			}
			array = array >> 1; /* ��λ���� */
			y += 1;				/* ��ʾ��y��λ�ü�һ */
		}

		x += 1;										 /* x�����Ҽ�һ */
		if ((size1 != 8) && ((x - x0) == size1 / 2)) /* �����һ��������ʾ����,��ʼд����һ�е����� */
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0; /* y��ֵ��8*/
	}
}

/**
 * ��������: ��ʾӢ���ַ���
 * �������: x �����x�� ��Χ:0-127
 * �������: y �����y�� ��Χ:0-31
 * �������: str ��Ҫ��ʾ���ַ���
 * �������: char_size ��ʾ�ַ����Ĵ�С
 * �������: mode ������ʾ/��ɫ��ʾ 1 ������ʾ 0 ��ɫ��ʾ
 * ˵    ��: ���� 8*6������
 */
void OLED_Show_String(uint8_t x, uint8_t y, uint8_t *str, uint8_t str_size, uint8_t mode)
{
	while ((*str >= ' ') && (*str <= '~'))
	{
		OLED_Show_Char(x, y, *str, str_size, mode);
		if(str_size == 8)x += 6;
		else x += str_size / 2;
		str++;
	}
}


/**
 * ��������: ��ֱ�ߺ���
 * �������: x0 �����x�� ��Χ:0-127
 * �������: y0 �����y�� ��Χ:0-31
 * �������: x1 �����x�� ��Χ:0-127
 * �������: y1 �����y�� ��Χ:0-31
 * ˵    ��: ��
 */
void OLED_Draw_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t dm = 0;
	uint8_t lx = abs(x1 - x0);
	uint8_t ly = abs(y1 - y0);
	double x = 0;;
	double y = 0;
	
	if(lx >= ly){dm = lx;}
	else dm = ly;
	
	float dx = (float)(x1 - x0) / dm;
	float dy = (float)(y1 - y0) / dm;
	
	x = (double)x0 + 0.5;
	y = (double)y0 + 0.5;
	
	for(uint8_t i=0; i<dm; i++)
	{
		OLED_DrawPoint(x, y, 1);
		x += dx;
		y += dy;
	}
}

/**
* ��������: ��ʾͼƬ
 * �������: x ͼƬ��x�����
 * �������: y ͼƬ��y�����
 * �������: bmp_x ͼƬ��x����
 * �������: bmp_y ͼƬ��y����
 * ˵    ��: ��
 */
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t bmp_x,uint8_t bmp_y,uint8_t BMP[],uint8_t mode)
{
	uint16_t j=0;
	
	uint8_t x0=x,y0=y;
	
	bmp_y=bmp_y/8+((bmp_y%8)?1:0);
	
	for(uint8_t n=0;n<bmp_y;n++)
	{
		 for(uint8_t i=0;i<bmp_x;i++)
		 {
				uint8_t temp=BMP[j];
				j++;
				for(uint8_t m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==bmp_x)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}

