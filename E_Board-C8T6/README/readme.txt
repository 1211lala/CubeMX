#include "./UART1_232/uart1_232.h"


#define UART1_ASK_CMD 0X55					/* ��ѯ���� */
#define UART1_DATA_CMD 0XAA 				/* �������� */
#define UART1_FRAME_HANDER 0XAAFF		/* ͷ֡*/
#define PRINTF_UART1  1


uint8_t uart1_send_ask_buf[20];		/* ROSѯ��֡���ص����� */
uint8_t uart1_send_data_buf[40];	/* ROS����֡���ص����� */


uint16_t Sensor1_data  = 1111;		 /* ���趨��Ĵ��������� */	
uint16_t Sensor2_data  = 2222; 
uint16_t Sensor3_data  = 3333; 
uint16_t Sensor4_data  = 4444; 
uint16_t Sensor5_data  = 5555; 
uint16_t Sensor6_data  = 6666; 
uint16_t Sensor7_data  = 1111;		 /* ���趨��Ĵ��������� */	
uint16_t Sensor8_data  = 2222; 
uint16_t Sensor9_data  = 3333; 
uint16_t Sensor10_data = 4444; 
uint16_t Sensor11_data = 5555; 
uint16_t Sensor12_data = 6666; 
uint16_t Sensor13_data = 1111;		 /* ���趨��Ĵ��������� */	
uint16_t Sensor14_data = 2222; 
uint16_t Sensor15_data = 3333; 
uint16_t Sensor16_data = 4444; 
uint16_t Sensor17_data = 5555; 
uint16_t Sensor18_data = 6666; 
uint16_t Sensor19_data = 6666; 
uint16_t Sensor20_data = 6666; 


uint16_t read_sensor(uint8_t code)
{
	uint16_t sensor_data = 0;
	switch(code)	/* ��ѯ��ȡ�����ֵ����� */
	{
		case(0x01): sensor_data = Sensor1_data; break;
		case(0x02): sensor_data = Sensor2_data; break;
		case(0x03): sensor_data = Sensor3_data; break;
		case(0x04): sensor_data = Sensor4_data; break;
		case(0x05): sensor_data = Sensor5_data; break;
		case(0x06): sensor_data = Sensor6_data; break;
		case(0x07): sensor_data = Sensor7_data; break;
		case(0x08): sensor_data = Sensor8_data; break;
		case(0x09): sensor_data = Sensor9_data; break;
		case(0x0A): sensor_data = Sensor10_data; break;
		case(0x0B): sensor_data = Sensor11_data; break;
		case(0x0C): sensor_data = Sensor12_data; break;
		case(0x0D): sensor_data = Sensor13_data; break;
		case(0x0E): sensor_data = Sensor14_data; break;
		case(0x0F): sensor_data = Sensor15_data; break;
		case(0x10): sensor_data = Sensor16_data; break;
		case(0x11): sensor_data = Sensor17_data; break;
		case(0x12): sensor_data = Sensor18_data; break;
		case(0x13): sensor_data = Sensor19_data; break;
		case(0x14): sensor_data = Sensor20_data; break;
		default:break;
	}
}

void MY_UART1_Idle_Callback(uint8_t size)
{
	uart1_rec_buf[size] = '\0';
	
	#if PRINTF_UART1
		printf("\r\n���յ�%d������\r\n", size);
		for(int i=0; i<size; i++)
		{
			printf("rec[%d]=0x%x  ", i, uart1_rec_buf[i]);
		}
	#endif
	
	uint16_t rec_header = (uart1_rec_buf[0] << 8) | uart1_rec_buf[1];
	if(rec_header == UART1_FRAME_HANDER)					/* �ж�֡ͷ�Ƿ��Ӧ */
	{
		
		#if PRINTF_UART1
			printf("\r\n֡ͷ���ճɹ�\r\n");
		#endif
		
		if(uart1_rec_buf[2] == UART1_ASK_CMD)				/* �ж�ROS���������Ƿ�Ϊѯ�ʴ������������� */
		{
			#if PRINTF_UART1
				printf("ѯ��������ճɹ�\r\n");
			#endif
			if(uart1_rec_buf[3] == 0x00)							/* �����ѯ�������3λΪ0x00 */
			{
				uint8_t rec_sum = test_crc_sum(uart1_rec_buf, size);	/* ���������size������strlen()��ã������п��ܻ���0x00�����ݻ�Ӱ��strlen()*/
				if(rec_sum == uart1_rec_buf[4])					/* �ж�crc_sum */
				{
					printf("���ߴ�������ŷ��ͳɹ�\r\n");
					/*�������ߴ�������ź���*/
				}
			}
		}
		
		
		else if(uart1_rec_buf[2] == UART1_DATA_CMD)	/* �ж�ROS���������Ƿ�Ϊ��ȡ�������������� */
		{
			#if PRINTF_UART1
				printf("����������ճɹ�\r\n");
			#endif
			uint8_t rec_code_num = uart1_rec_buf[3];	/* �õ�ROS��Ҫ�������ĵ����� �������*/
			uint8_t count =3;													/* �����������������е���ʼλ,Ӧ����4���������forѭ���м�һ��*/
			uart1_send_data_buf[0] = 0xaa;						/* ���巢��֡ͷ */
			uart1_send_data_buf[1] = 0xff;
			uart1_send_data_buf[2] = 0xaa;						/* �������������� */
			uart1_send_data_buf[3] = rec_code_num*2;	/* ���巢�����ݳ��� */
			
			for(int i=4; i<4+rec_code_num; i++)
			{
				uint16_t read;
				read = read_sensor(uart1_rec_buf[i]);
				count += 1;
				uart1_send_data_buf[count] = read >> 8;
				count += 1;
				uart1_send_data_buf[count] = read;
			}
			uart1_send_data_buf[count+1] = get_crc_sum(uart1_send_data_buf, count+2);
			printf("%s",uart1_send_data_buf);

			
			
			
			#if PRINTF_UART1
				uint8_t rec_code_buf[6];									/* ���洫���Ĵ�������� */				
				for(int i=0; i<rec_code_num; i++)			
				{
					rec_code_buf[i] = uart1_rec_buf[3+1+i];	/* ��������ͬʱ����˳�� */
				}
				printf("���յ�%d��������\r\n", rec_code_num);
				for(int i=0; i<rec_code_num; i++)
				{
					printf("rec[%d]=0x%x  ", i, rec_code_buf[i]);
				}
			#endif
		}
	}
}













