#include "text.h"

/* ��ʾ����GB2312 ��λ�ķ�Χ�� 0xa1 - 0xf7  ��λ�ķ�ΧΪ 0xa1 - 0xfe */
/* OLED��ȡģ��ʽΪ ���� ����ʽ */




/**
 * ��������: ��ʾһ������
 * �������: x ��ʾ��x��
 * �������: y ��ʾ��y��
 * �������: size �����С ������8��������
 * �������: buff ��������
 */
void FS_Show_Picture_Chinese(uint8_t x, uint8_t y, uint8_t size, uint8_t* buff, uint8_t mode)
{
	uint8_t x0, y0;
	x0 = x;
	y0 = y;
	for (uint8_t i = 0; i < size*size/8; i++)	/* д���������� */
	{
		for (uint8_t j = 0; j < 8; j++) 
		{
			if (buff[i] & 0x01) /* ��λ��ȡ�ֽ����� */
			{
				OLED_DrawPoint(x, y, mode);
			}else
			{
				OLED_DrawPoint(x, y, !mode);
			}
			buff[i] = buff[i] >> 1; 
			y += 1;				
		}
		x += 1;								/* x�����Ҽ�һ */
		if (x - x0 == size) 	/* �����һ��������ʾ����,��ʼд����һ�е����� */
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
	}
}


/**
 * ��������: ��ʾ���� ������ʾ��û����
 * �������: x ��ʾ��x��
 * �������: y ��ʾ��y��
 * �������: size �����С ������8��������
 * �������: buff ��������
 */
uint8_t FS_Show_Text(uint8_t x, uint8_t y, uint8_t size, uint8_t* gbk, uint8_t mode)
{
	
	switch(size)	
	{
		case(16):	fs_status =  f_open(FSFile, "H_16.HZK", FA_READ); break;
		case(24):	fs_status =  f_open(FSFile, "H_24.HZK", FA_READ); break;
		case(32):	fs_status =  f_open(FSFile, "H_32.HZK", FA_READ); break;
	}
	if( fs_status == FR_OK )
	{
		for(uint16_t i=0; i<strlen((char*)gbk); )
		{
			if(gbk[i] > 0x80)		/* �ж��Ƿ�ΪGB2312 */
			{
				uint32_t addr;
				switch(size)			/* ��ȡ�����С ������һ�������С */
				{
					case(16): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 32;	break;
					case(24): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 72;	break;
					case(32): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 128;	break;
				}
				f_lseek(FSFile, addr);		/* ������������ƫ�� */
				 
				if(f_read(FSFile, fs_buf, size*size/8,  &fs_r_count)  == FR_OK )
				{
					FS_Show_Picture_Chinese(x, y, size, fs_buf, mode);	/* ʵ��д������ */
				}
				x += size;	/* ����ÿ����֮��Ŀ�϶ */
				i += 2;			/* GB2312�������ֽ� */
			}else	/* �ж��Ƿ�ΪASCLL */
			{
				OLED_Show_Char(x, y, gbk[i], size, mode);	/* ʵ��д������ */
				x += size/2;	/* ����ÿ��ASCLL֮��Ŀ�϶ */
				i += 1;		/* ASCLL��һ���ֽ� */
			}
		}
		f_close(FSFile);
	}else
	{
		return 0;
	}
	return 1;
}

/**
 * ��������: ��ʾ��ɫͼƬ
 * �������: x ��ʾ��x��
 * �������: y ��ʾ��y��
 * �������: size �����С ������8��������
 * �������: buff ��������
 */
uint8_t FS_Show_Picture(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end,uint8_t * path, uint32_t size, uint8_t mode)
{
	fs_status = f_open(FSFile, (const TCHAR*)path, FA_READ);
	if(fs_status == FR_OK)
	{
		if(f_read(FSFile, fs_buf, size,  &fs_r_count)  == FR_OK )
		{
			OLED_ShowPicture(x_start, y_start, x_end, y_end, fs_buf, mode);
			OLED_Refresh();
		}
		f_close(FSFile);
	}else
	{
		return 0;
	}
	return 1;
}

