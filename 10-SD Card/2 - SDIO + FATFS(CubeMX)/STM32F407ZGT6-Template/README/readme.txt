����ʱ��2023.9.22 

�޸�ʱ�� -- 2023.9.25 ȡ����ʹ�� use Micro LIB �� Ϊ�˽���������в��˵�������private.c��������´���

	/* ��֪����������C������ʹ�ð������ĺ��� */
	#pragma import(__use_no_semihosting)
	 
	/* ���� _sys_exit() �Ա���ʹ�ð�����ģʽ */
	void _sys_exit(int x)
	{
		x = x;
	}
	 
	/* ��׼����Ҫ��֧������ */
	struct __FILE
	{
		int handle;
	};
	 
	FILE __stdout;
	
	
�ο����� https://blog.csdn.net/qq_36098477/article/details/117666808  



/********************************************************************************************************************************************************************/


CubeMX����SD��bug
	��ѡ��������SDIO�ӿ�ʱMX���ɵĴ���Ĳ����� SDIO_BUS_WIDE_4B
	�����ڳ�ʼ��ʱ����һ��ʼ���� 4��ģʽ��Ӧ���ֶ��ĳ� SDIO_BUS_WIDE_1B
	�ڵ��� HAL_SD_Init() ������ �ٵ��� if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)����Ϊ4��ģʽ
	��Ȼ�����ϵͳ������ȥ
	
	SDIO_TRANSFER_CLK_DIV HAL������Ƕ����һ������SDIO�ĺ� ��� 48/0+2 =24Mhz 
	���ڳ�ʼ��ʱ��Ƶ�ʲ��ܸ���400K��SDIO_INIT_CLK_DIV �������ʱHAL���Լ����Ĳ������ǿ��� 

	
	
CubeMX���ɵ�FATFS  
	�� sd_diskio.c�ж����˽ӿں���
	static DSTATUS SD_CheckStatus(BYTE lun);
	DSTATUS SD_initialize(BYTE lun);
	DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count);
	DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count);
	
	
	
	
	