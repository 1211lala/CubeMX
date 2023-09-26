#include "./IIC1/iic1.h"

/**
 * @brief       ����I2C�ٶȵ���ʱ
 * @param       ��
 * @retval      ��
 */
static void iic1_delay(void)
{
    my_delay_us(2);
}

/**
 * @brief       IIC�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void iic1_init(void)
{
    iic1_start();  /* ֹͣ�����������豸 */
}

/**
 * @brief       ����IIC��ʼ�ź�
 * @param       ��
 * @retval      ��
 */
void iic1_start(void)
{
    IIC1_SDA(1);
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SDA(0);      /* START�ź�: ��SCLΪ��ʱ, SDA�Ӹ߱�ɵ�, ��ʾ��ʼ�ź� */
    iic1_delay();
    IIC1_SCL(0);      /* ǯסI2C���ߣ�׼�����ͻ�������� */
    iic1_delay();
}

/**
 * @brief       ����IICֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
void iic1_stop(void)
{
    IIC1_SDA(0);      /* STOP�ź�: ��SCLΪ��ʱ, SDA�ӵͱ�ɸ�, ��ʾֹͣ�ź� */
    iic1_delay();
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SDA(1);      /* ����I2C���߽����ź� */
    iic1_delay();
}

/**
 * @brief       �ȴ�Ӧ���źŵ���
 * @param       ��
 * @retval      1������Ӧ��ʧ��
 *              0������Ӧ��ɹ�
 */
uint8_t iic1_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;
    
    IIC1_SDA(1);      /* �����ͷ�SDA��(��ʱ�ⲿ������������SDA��) */
    iic1_delay();
    IIC1_SCL(1);      /* SCL=1, ��ʱ�ӻ����Է���ACK */
    iic1_delay();

    while (IIC1_READ_SDA()) /* �ȴ�Ӧ�� */
    {
        waittime++;

        if (waittime > 250)
        {
            iic1_start();
            rack = 1;
            break;
        }

        iic1_delay();
    }

    IIC1_SCL(0);      /* SCL=0, ����ACK��� */
    iic1_delay();
    return rack;
}


/**
 * @brief       ����ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic1_ack(void)
{
    IIC1_SDA(0);  /* SCL 0 -> 1  ʱSDA = 0,��ʾӦ�� */
    iic1_delay();
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SCL(0);
    iic1_delay(); 
    IIC1_SDA(1);  /* �����ͷ�SDA�� */
    iic1_delay(); 
}

/**
 * @brief       ������ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic1_nack(void)
{
    IIC1_SDA(1);  /* SCL 0 -> 1  ʱ SDA = 1,��ʾ��Ӧ�� */
    iic1_delay();
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SCL(0);
    iic1_delay();
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       data: Ҫ���͵�����
 * @retval      ��
 */
void iic1_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC1_SDA((data & 0x80) >> 7); /* ��λ�ȷ��� */
        iic1_delay();
        IIC1_SCL(1);
        iic1_delay();
        IIC1_SCL(0);
        data <<= 1;     /* ����1λ,������һ�η��� */
    }

    IIC1_SDA(1);      /* �������, �����ͷ�SDA�� */
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       ack:  ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t iic1_read_byte(unsigned char ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* ����1���ֽ����� */
    {
        receive <<= 1;          /* ��λ�����,�������յ�������λҪ���� */
        IIC1_SCL(1);
        iic1_delay();

        if (IIC1_READ_SDA())
        {
            receive++;
        }
        
        IIC1_SCL(0);
        iic1_delay();

    }

    if (!ack)
    {
        iic1_nack();  /* ����nACK */
    }
    else
    {
        iic1_ack();   /* ����ACK */
    }

    return receive;
}







