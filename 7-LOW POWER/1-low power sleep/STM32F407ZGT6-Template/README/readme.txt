����ʱ��2023.9.26  ��������:ʹ��SSD1306����128*32��oled��Ļ(���IIC) 	
�ο����� https://www.bilibili.com/video/BV19u4y197df/?spm_id_from=333.1007.top_right_bar_window_history.content.click







�޸Ĺ��� --  ȡ����ʹ�� use Micro LIB �� Ϊ�˽���������в��˵�������private.c��������´���

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
	
	
�ο����� https://blog.csdn.net/qq_36098477/article/details/11766 