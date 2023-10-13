#ifndef _W25Q128_H_
#define _W25Q128_H_

#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "usart.h"
/********************** ���ݵ�оƬID ************************/
#define W25Q80 		0XEF13
#define W25Q16 		0XEF14
#define W25Q32 		0XEF15
#define W25Q64 		0XEF16
#define W25Q128		0XEF17
#define W25Q256 	0XEF18

#define NM25Q80 	0X5213
#define NM25Q16 	0X5214
#define NM25Q32 	0X5215
#define NM25Q64 	0X5216
#define NM25Q128	0X5217
#define NM25Q256 	0X5218

/********************* ����ʹ�õ�Flash **********************/
extern uint16_t W25QXX_TYPE;	

/********************* ����ʹ�õ�Ƭѡ ***********************/
/*�ر�Ƭѡ*/
#define W25QXX_CS_HIGH    HAL_GPIO_WritePin(W25Q128_CS_GPIO_Port,W25Q128_CS_Pin,GPIO_PIN_SET)
/*��Ƭѡ*/
#define W25QXX_CS_LOW     HAL_GPIO_WritePin(W25Q128_CS_GPIO_Port,W25Q128_CS_Pin,GPIO_PIN_RESET)


//ָ���
#define W25X_WriteEnable				0x06 	/* дʹ�ܣ�д�����ݲ�������ǰ���뷢������ָ�� */
#define W25X_WriteDisable				0x04 
#define W25X_ReadStatusReg1			0x05	/* ��SR1�Ĵ�����ͨ������ֵ�ж�оƬ�ǲ��Ǵ��ڿ���״̬ */
#define W25X_ReadStatusReg2			0x35 
#define W25X_ReadStatusReg3			0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData						0x03	/* ������ָ����ڶ�ȡFlash������ */
#define W25X_FastReadData				0x0B 
#define W25X_FastReadDual				0x3B 
#define W25X_PageProgram				0x02	/* ҳдָ����д��256���ֽ����ݣ����˻Ḵд */
#define W25X_BlockErase					0xD8 
#define W25X_SectorErase				0x20 	/* ������������С����4096���ֽڣ�4Kyte��*/
#define W25X_ChipErase					0xC7 
#define W25X_PowerDown					0xB9 
#define W25X_ReleasePowerDown		0xAB 
#define W25X_DeviceID						0xAB 
#define W25X_ManufactDeviceID		0x90	/* ��ȡ������ID(�ֲ�51ҳ)*/
#define W25X_JedecDeviceID			0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9


uint8_t SPI1_ReadWriteByte(uint8_t TxData);
uint16_t W25QXX_ReadID(void);
uint8_t W25QXX_ReadSR(uint8_t regno);
void W25QXX_Write_SR(uint8_t regno,uint8_t sr);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
void W25QXX_Wait_Busy(void);
void W25QXX_Erase_Chip(void) ;
void W25QXX_Erase_Sector(uint32_t Dst_Addr);


void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);

void W25QXX_Init(void);
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);


#endif



/*****************************************************************

W25Q128��SPIģʽΪģʽ0��ģʽ3���������ģʽ0
���ݸ�ʽΪ8λ����һ��bitΪMSB

һ��W25Q128��256�Ŀ�
һ������16������ 	(���� == ��С�ĵĲ�����λ)
һ��������16��ҳ 	(ҳ == ��С�ĵ�д�뵥λ)
һ��ҳ��256���ֽ� 

��ַ�� 0x000000 - 0xffffff

��״̬�Ĵ���SR1 
	����CS --> ����0x05ָ�� --> ����SR1��ֵ --> ����Ƭѡ
	
ҳдʱ��
	����CS --> ����0x02ָ�� --> ����24λ��ַ --> ����n������(n�ķ�ΧΪ1-256) --> ����Ƭѡ 
	
��������ָ��
	����CS --> ����0x20ָ�� --> ����24δ��ַ --> ����Ƭѡ


�ڲο��ֲ�13ҳ
״̬�Ĵ���SR1��ֵ bit0(BUSY) 0������״̬	1��æµ״̬
״̬�Ĵ���SR1��ֵ bit1(WEL) 0��д��ֹ	1��дʹ��

D0 --- MISO
D1 --- MOSI


ע�� ����W25Q128д���������(0.5kb - nkb֮��Ҫ������û�п���ջ���)
stm32CubeMXĬ�ϵ�ջ��СΪ 512b
д����ʱ��Ϊ������һ��4k��ȫ�����飬���Բ��ÿ���
�ڶ�����ʱҪ�ر�ע�⣬����Ҫ��һ��1k�ֽڵ����ݣ���Ҫ��ջ��С���ó�2K����(�����������õģ���������ʹ�ã�С�˾Ͳ������������ݣ�ͬʱ����Ҳ����)


STM32��SPI1���ٶ��ֲ��Ϲ涨���18Mhz,ʵ�������36MhzҲ���ԣ������ȶ�����18Mhz
*****************************************************************/

