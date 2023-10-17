#include "fstest.h"


/**
 * @brief       �鿴����·���µ��ļ����ļ��Ӳ�֧����Ȳ鿴,��֧������·��,���������ڲ鿴
 * @param       path : ·��
 * @retval      ִ�н��(�μ�FATFS, FRESULT�Ķ���)
 */
FRESULT fs_scan_dir(uint8_t* path)
{
	fs_status = f_opendir(FSdir, (const TCHAR*)path);
	if(fs_status == FR_OK)	/* Ŀ¼�򿪳ɹ� */
	{
		while(1)
		{
			fs_status = f_readdir(FSdir, FSinfo);
			if((fs_status != FR_OK) || (FSinfo->fname[0] == 0))
			{
				break;
			}
//			if(FSinfo->fname[0] == '.')
//			{
//				continue;
//			}
			printf("%s/",path);
			printf("%s\r\n", FSinfo->fname);
		}
		f_closedir(FSdir);
		return FR_OK;
	}else
	{
		printf("%s open fail! \r\n", path);
		return FR_DISK_ERR;
	}
}

/**
 * @brief       �õ����̵Ĵ�С��ʣ���С,���ڴ�ӡ����
 * @param       path : ·��
 * @retval      ִ�н��(�μ�FATFS, FRESULT�Ķ���)
 */
FRESULT fs_get_free(uint8_t* path)
{
	uint32_t fre_clust = 0;
	fs_status = f_getfree((const TCHAR*)path, (DWORD*)&fre_clust, &SDFATFS);
	if(fs_status == FR_OK)
	{
		uint32_t total = (SDFATFS->n_fatent - 2) * SDFATFS->csize;	/* �õ��������� */
		uint32_t free  = fre_clust * SDFATFS->csize;          			/* �õ����������� */
		#if FF_MAX_SS!=512
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
		#endif
		if (total < 20480)   /* ������С��20M */
		{
			printf("\r\n����������:%d KB\r\n""���ÿռ�:%d KB\r\n", total >> 1, free >> 1);
		}else
    {
			printf("\r\n����������:%d MB\r\n""���ÿռ�:%d MB\r\n",total >> 11, free >> 11);
		}
		return FR_OK;
	}else
	{
		printf("%s get free error.\r\n", path);
		return FR_DISK_ERR;
	}
}



void sd_test(void)
{
	fs_scan_dir((uint8_t*)"0:");
	fs_get_free((uint8_t*)"0:");
}


void flash_test(void)
{
	printf("*********************************FLASH********************************\r\n");
	f_mount(FLASHFATFS, "1:", 0);
	//�����ļ���
	fs_status = f_open(FSFile, "1:flash.txt", FA_CREATE_NEW);
	if( fs_status == FR_OK )
	{
		printf("1:flash.txt creat ok.  \r\n");
		f_close(FSFile);
	}else
	{
		printf("1:flash.txt creat fail.  error code %d\r\n", fs_status);
	}
	fs_status = f_open(FSFile, "1:flash.txt", FA_WRITE);
	if( fs_status == FR_OK )
	{
		sprintf((char*)fs_buf, "1234567890");
		fsize = f_size(FSFile);
		f_lseek(FSFile, fsize);
		fs_status = f_write(FSFile, fs_buf, strlen((char*)fs_buf), &fs_w_count);
		if(fs_status == FR_OK)
		{
			printf("д���С %d\r\n", strlen((char*)fs_buf));
			printf("д������ %s\r\n", fs_buf);
		}
		f_close(FSFile);
	}else
	{
		printf("1:flash.txt write fail.  error code %d\r\n", fs_status);
	}


	fs_status = f_open(FSFile, "1:flash.txt", FA_READ);
	if( fs_status == FR_OK )
	{
		fsize = f_size(FSFile);
		/* ���ݶ�ȡ */
		fs_status = f_read(FSFile, fs_buf, fsize, &fs_r_count);
		if(fs_status == FR_OK)
		{
			fs_buf[fs_r_count] = '\0';
			printf("�ļ���С %d\r\n", fsize);
			printf("������С %d\r\n", fs_r_count);
			printf("%s\r\n\r\n", fs_buf);
		}
		f_close(FSFile);
	}else
	{
		printf("1:flash.txt read fail.  error code %d\r\n", fs_status);
	}
}

