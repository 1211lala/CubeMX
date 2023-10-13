#include "./W25Q128/w25q128.h"


uint16_t W25QXX_TYPE = W25Q256;
/**
 * @brief SPI1��дһ���ֽ�
 * @param TxData Ҫд����ֽ�
 * @return �����յ�������
 */
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
  uint8_t Rxdata;
	
	HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    	
}

/*��ȡоƬID
	0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
	0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
	0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
	0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
	0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
	0XEF18,��ʾоƬ�ͺ�ΪW25Q256 
*/
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25QXX_CS_LOW;			/* Ƭѡ���� */	    
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	W25QXX_CS_HIGH;			/* Ƭѡ���� */	  
	#if 0
	printf("W25Q128 ID = 0X%X\r\n",Temp);
	#endif
	return Temp;
}   


/**��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
	״̬�Ĵ���1��
	BIT7  6   5   4   3   2   1   0
	SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
	TB,BP2,BP1,BP0:FLASH����д��������
	WEL:дʹ������
	BUSY:æ���λ(1,æ;0,����)
	Ĭ��:0x00
	״̬�Ĵ���2��
	BIT7  6   5   4   3   2   1   0
	SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
	״̬�Ĵ���3��
	BIT7      6    5    4   3   2   1   0
	HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
	regno:״̬�Ĵ����ţ���:1~3
	����ֵ:״̬�Ĵ���ֵ
*/
uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
	switch(regno)
	{
			case 1:
					command=W25X_ReadStatusReg1;    /* ��״̬�Ĵ���1ָ�� */
					break;
			case 2:
					command=W25X_ReadStatusReg2;    /* ��״̬�Ĵ���2ָ�� */
					break;
			case 3:
					command=W25X_ReadStatusReg3;    /* ��״̬�Ĵ���3ָ�� */
					break;
			default:
					command=W25X_ReadStatusReg1;    
					break;
	}    
	W25QXX_CS_LOW;                          /* ʹ������ */    
	SPI1_ReadWriteByte(command);            /* ���Ͷ�ȡ״̬�Ĵ������� */    
	byte=SPI1_ReadWriteByte(0Xff);          /* ��ȡһ���ֽ� */  
	W25QXX_CS_HIGH;                         /* ȡ��Ƭѡ */     
	return byte;   
} 


/**
 * @brief дW25QXX״̬�Ĵ���
 * @param regno Ҫд���״̬�Ĵ���
 * @param sr    Ҫд�������
 * @return ��
 */
void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{   
	uint8_t command=0;
	switch(regno)
	{
			case 1:
					command=W25X_WriteStatusReg1;    /* д״̬�Ĵ���1ָ�� */
					break;
			case 2:
					command=W25X_WriteStatusReg2;    /* д״̬�Ĵ���2ָ�� */
					break;
			case 3:
					command=W25X_WriteStatusReg3;    /* д״̬�Ĵ���3ָ�� */
					break;
			default:
					command=W25X_WriteStatusReg1;    
					break;
	}   
	W25QXX_CS_LOW;                          /* ʹ������ */     
	SPI1_ReadWriteByte(command);            /* ���Ͷ�ȡ״̬�Ĵ������� */    
	SPI1_ReadWriteByte(sr);                 /* ��ȡһ���ֽ� */    
	W25QXX_CS_HIGH;                         /* ȡ��Ƭѡ */  	      
}   


/**
 * @brief W25QXXдʹ��	��WEL��λ 
 */
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS_LOW;                           
  SPI1_ReadWriteByte(W25X_WriteEnable);     
	W25QXX_CS_HIGH;                           	      
} 

/**
 * @brief W25QXXд��ֹ	��WEL���� 
 */
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS_LOW;                          
  SPI1_ReadWriteByte(W25X_WriteDisable);  
	W25QXX_CS_HIGH;                          	      
}

/**
 * @brief �������ģʽ
 */
void W25QXX_PowerDown(void)   
{ 
	W25QXX_CS_LOW;                           /* ʹ������ */   
	SPI1_ReadWriteByte(W25X_PowerDown);      /* ���͵������� */  
	W25QXX_CS_HIGH;                          /* ȡ��Ƭѡ */     	      
	HAL_Delay(1);                     			 /* �ȴ�TPD Delay_us(&htim6, 3)*/
} 

/**
 * @brief �˳�����ģʽ
 */
void W25QXX_WAKEUP(void)   
{  
	W25QXX_CS_LOW;                                 /* ʹ������ */   
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);     /* �����˳��������� */  
	W25QXX_CS_HIGH;                                /* ȡ��Ƭѡ */   	      
	HAL_Delay(1);                     			       /* �ȴ�TRES1 Delay_us(&htim6, 3)*/                    
}    
/**
 * @brief �ȴ�����
 */
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   //�ȴ�BUSYλ���
}  

/**
 * @brief ��������оƬ	�ȴ�ʱ�䳬�� 
 */
void W25QXX_Erase_Chip(void)   
{                                   
  W25QXX_Write_Enable();                 /* ���ȷ���дʹ�� */
  W25QXX_Wait_Busy();   								 /* �ȴ����� */
	W25QXX_CS_LOW;                         /* ʹ������ */   
	SPI1_ReadWriteByte(W25X_ChipErase);    /* ����Ƭ�������� */  
	W25QXX_CS_HIGH;                        /* ȡ��Ƭѡ */     	      
	W25QXX_Wait_Busy();   				   			 /* �ȴ�оƬ�������� */
} 


/**
 * @brief ����һ����������ʱ��:150ms
 * @param Dst_Addr ������ַ ����ʵ����������
 * @return ��
 */
void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	Dst_Addr*=4096;
	W25QXX_Write_Enable();                  /* ���ȷ���дʹ�� */
	W25QXX_Wait_Busy();   									/* �ȴ����� */
	W25QXX_CS_LOW;                          /* ʹ������ */   
	SPI1_ReadWriteByte(W25X_SectorErase);   /* ���������������� */  
	if(W25QXX_TYPE==W25Q256)                /* �����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ */
	{
			SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>24)); /* ����32bit��ַ */   
	}
	SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  /* ����24bit��ַ */   
	SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
	SPI1_ReadWriteByte((uint8_t)Dst_Addr);  
	W25QXX_CS_HIGH;                            	/* ȡ��Ƭѡ */     	      
	W25QXX_Wait_Busy();   				       				/* �ȴ�������� */
}  



/**
 * @brief �趨��4�ֽڵ�ַ����3�ֽ�
 * @note  SP1��NSS���������,��gpio.c�б�д
 */
void W25QXX_Init(void)
{ 
	uint8_t temp;
	W25QXX_CS_HIGH;			                	/* SPI FLASH��ѡ�� */
	W25QXX_TYPE=W25QXX_ReadID();	        /* ��ȡFLASHID */
	if(W25QXX_TYPE==W25Q256)              /* SPI FLASHΪW25Q128 */
	{
		temp=W25QXX_ReadSR(3);              /* ��ȡ״̬�Ĵ���3���жϵ�ַģʽ */
		if((temp&0X01)==0)			        		/* �������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ */
		{
			W25QXX_CS_LOW; 			        
			SPI1_ReadWriteByte(W25X_Enable4ByteAddr);	/*���ͽ���4�ֽڵ�ַģʽָ�� */  
			W25QXX_CS_HIGH;       		     
		}
	}
}  


/**
 * @brief ��ȡSPI FLASH ����
 * @param pBuffer ���ݴ洢��
 * @param ReadAddr ��ʼ��ȡ�ĵ�ַ(24bit)
 * @param NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
 * @return ��
 */
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	W25QXX_CS_LOW;                          	  
	SPI1_ReadWriteByte(W25X_ReadData);      
	if(W25QXX_TYPE==W25Q256)                
	{
		SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>24));    
	}
	SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>16));       
	SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
	SPI1_ReadWriteByte((uint8_t)ReadAddr);  
	
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=SPI1_ReadWriteByte(0XFF);    /* ѭ������ */ 
	}
	W25QXX_CS_HIGH;  				    	      
}  

/**
 * @brief ҳд - ��ָ����ַ��ʼд�����256�ֽڵ�����
 * @param pBuffer ���ݴ洢��
 * @param ReadAddr ��ʼд��ĵ�ַ(24bit)
 * @param NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	
 * @return ��
 */
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  W25QXX_Write_Enable();                  /* ���ȷ���дʹ�� */
	W25QXX_CS_LOW;                            
	SPI1_ReadWriteByte(W25X_PageProgram);   /* ����дҳ���� */   
	if(W25QXX_TYPE==W25Q256)                /* �����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ */
	{
			SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>24)); 
	}
	SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); 
	SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
	SPI1_ReadWriteByte((uint8_t)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)
	{
		SPI1_ReadWriteByte(pBuffer[i]);/* ѭ��д�� */ 
	}
	W25QXX_CS_HIGH;                  /* ȡ��Ƭѡ */
	W25QXX_Wait_Busy();					     /* �ȴ�д����� */
} 

/**
 * @brief ҳд - �޼���дSPI FLASH,�����Զ���ҳ����,��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
 * @param pBuffer ���ݴ洢��
 * @param ReadAddr ��ʼд��ĵ�ַ(24bit)
 * @param NumByteToWrite Ҫд����ֽ���(���65535)!	
 * @return ��
 */
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; 	/* �õ���ַ����ҳ��ʣ����ֽ��� */		 	    
	if(NumByteToWrite<=pageremain)	/* �����ҳ��д�� */
	{
		pageremain=NumByteToWrite;	
	}
	while(1)
	{	 
		/*
		(1): �����ڱ�ҳд�꣬��ֱ��д���˳�		
		(2): ��ҳд����
					1.�Ȱѱ�ҳ������д�� W25QXX_Write_Page(pBuffer,WriteAddr,pageremain)
					2.д����������Ҫƫ���Ѿ�д�����ݸ��� pBuffer+=pageremain;
					3.д���ַҪ�����Ѿ�д�����ݵĸ��� WriteAddr+=pageremain;	
					4.��д�����ݸ���Ҫ��ȥ�Ѿ�д�����ݸ��� NumByteToWrite-=pageremain;ͬʱ�ж��Ƿ�С��256�����С��256һҳ�Ϳ���д���ˣ�����(1)����
					5.�ٴ�ִ��1.ѭ��д��
		*/
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);	
		if(NumByteToWrite==pageremain)	/* ��ҳ����д�� */
		{
			break;
		}
	 	else /* ��ҳʣ���ַд���� */
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;		/* ��ȥ�Ѿ�д���˵��ֽ��� */
			if(NumByteToWrite>256)
			{
				pageremain=256; 						/* һ�ο���д��256���ֽ� */
			}
			else 
			{
				pageremain=NumByteToWrite; 	/* ����256���ֽ��� */
			}
		}
	};   
}

/**
 * @note 	�ú�������������!	
 * @brief дSPI FLASH����ָ����ַ��ʼд��ָ�����ȵ�����
 * @param pBuffer ���ݴ洢��
 * @param WriteAddr ��ʼд��ĵ�ַ(24bit)
 * @param NumByteToWrite Ҫд����ֽ���(���65535)!	
 * @return ��
 */
uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;	/* ������ַ */ 
	secoff=WriteAddr%4096;	/* �������ڵ�ƫ�� */
	secremain=4096-secoff;	/* ����ʣ��ռ��С */   
 	//printf("ad:%X,nb:%d\r\n",WriteAddr,NumByteToWrite);/* ������ */
 	if(NumByteToWrite<=secremain)	/*�����ڵ�ʣ���ַ���� */
	{
		secremain=NumByteToWrite;
	}
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);/* ������������������ */
		for(i=0;i<secremain;i++)/* У������ */
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;/* ��Ҫ����������� */  	  
		}
		if(i<secremain)/* ��⵽���������Ѿ���д���ĵ�ַ */
		{
			W25QXX_Erase_Sector(secpos);/* ����������� */
			for(i=0;i<secremain;i++)	  /* ���� */
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);/* д����������  */ 

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);/* д�Ѿ������˵�,ֱ��д������ʣ������ */
		if(NumByteToWrite==secremain)break;	/* д������� */
		else	/* д��δ���� */
		{
			secpos++;	/* ������ַ��1 */
			secoff=0;	/* ƫ��λ��Ϊ0 */ 	 

			pBuffer+=secremain;  	/* ָ��ƫ�� */
			WriteAddr+=secremain;	/* д��ַƫ�� */	   
			NumByteToWrite-=secremain;							/* �ֽ����ݼ� */
			if(NumByteToWrite>4096)secremain=4096;	/* ��һ����������д���� */
			else secremain=NumByteToWrite;					/* ��һ����������д���� */
		}	 
	};	 
}








