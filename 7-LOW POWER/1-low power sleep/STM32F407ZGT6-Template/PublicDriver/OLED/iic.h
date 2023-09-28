#ifndef __IIC_H
#define __IIC_H


#include "main.h"
#include "gpio.h"
/******************************************************************************************/
/* ���� ���� */
#define IIC_SCL_PORT               			GPIOB
#define IIC_SCL_PIN                			GPIO_PIN_5

#define IIC_SDA_PORT               			GPIOB
#define IIC_SDA_PIN                			GPIO_PIN_6

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
void iic_init(void);            /* ��ʼ��IIC��IO�� */
void iic_start(void);           /* ����IIC��ʼ�ź� */
void iic_stop(void);            /* ����IICֹͣ�ź� */
void iic_ack(void);             /* IIC����ACK�ź� */
void iic_nack(void);            /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);     /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t txd);/* IIC����һ���ֽ� */
uint8_t iic_read_byte(unsigned char ack);/* IIC��ȡһ���ֽ� */

#endif

