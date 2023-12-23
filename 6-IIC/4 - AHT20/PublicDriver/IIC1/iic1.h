#ifndef __IIC_H
#define __IIC_H


#include "main.h"
#include "gpio.h"
/******************************************************************************************/
/* ���� ���� */
#define IIC_SCL_PORT               			GPIOA
#define IIC_SCL_PIN                			GPIO_PIN_3

#define IIC_SDA_PORT               			GPIOA
#define IIC_SDA_PIN                			GPIO_PIN_1

/******************************************************************************************/
/* IO���� */
#define IIC_SCL(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SCL */

#define IIC_SDA(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SDA */

#define IIC_READ_SDA     	HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN) /* ��ȡSDA */

/******************************************************************************************/
													
													
/* IIC���в������� */
void iic1_init(void);            /* ��ʼ��IIC��IO�� */
void iic1_start(void);           /* ����IIC��ʼ�ź� */
void iic1_stop(void);            /* ����IICֹͣ�ź� */
void iic1_ack(void);             /* IIC����ACK�ź� */
void iic1_nack(void);            /* IIC������ACK�ź� */
uint8_t iic1_wait_ack(void);     /* IIC�ȴ�ACK�ź� */
void iic1_send_byte(uint8_t txd);/* IIC����һ���ֽ� */
uint8_t iic1_read_byte(unsigned char ack);/* IIC��ȡһ���ֽ� */

#endif
