#include "./SDIO_CARD/sdio_card.h"


/**
 * @brief       ��ȡ����Ϣ����
 * @param       cardinfo:SD����Ϣ���
 * @retval      ����ֵ:��ȡ����Ϣ״ֵ̬
 */
uint8_t get_sd_card_info(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
    
    sta = HAL_SD_GetCardInfo(&hsd, cardinfo);
    
    return sta;
}

/**
 * @brief ͨ�����ڴ�ӡsd����Ϣ
 * 
 */
void show_sdcard_info(void)
{
    HAL_SD_CardCIDTypeDef   sd_card_cid;            /* CID�ṹ�� */
    HAL_SD_CardInfoTypeDef  g_sd_card_info_handle;  /* SD����Ϣ�ṹ�� */

    HAL_SD_GetCardCID(&hsd, &sd_card_cid); 
    get_sd_card_info(&g_sd_card_info_handle);           

    switch (g_sd_card_info_handle.CardType)
    {
    case CARD_SDSC:
    {
        if (g_sd_card_info_handle.CardVersion == CARD_V1_X)
        {
            printf("Card Type:SDSC V1\r\n");
        }
        else if (g_sd_card_info_handle.CardVersion == CARD_V2_X)
        {
            printf("Card Type:SDSC V2\r\n");
        }
    }
    break;

    case CARD_SDHC_SDXC:
        printf("Card Type:SDHC\r\n");
        break;
    default: break;
    }

    printf("������ID:%d\r\n", sd_card_cid.ManufacturerID);                         /*������ID */
    printf("����Ե�ַ:%d\r\n", g_sd_card_info_handle.RelCardAdd);                 /*����Ե�ַ */
    printf("�߼�������:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockNbr));   /*��ʾ�߼������� */
    printf("�߼����С:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockSize));  /*��ʾ�߼����С */
    printf("SD������:%d MB\r\n", (uint32_t)SD_TOTAL_SIZE_MB(&hsd));                /*��ʾ���� */
    printf("���С:%d\r\n\r\n", g_sd_card_info_handle.BlockSize);                  /*��ʾ���С */
}



/**
 * @brief       �ж�SD���Ƿ���Դ���(��д)����
 * @param       ��
 * @retval      ����ֵ:SD_TRANSFER_OK      ������ɣ����Լ�����һ�δ���
                       SD_TRANSFER_BUSY SD ����æ�������Խ�����һ�δ���
 */
uint8_t get_sd_card_state(void)
{
    return ((HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

/**
 * @brief       ��SD��(fatfs/usb����)
 * @param       pbuf  : ���ݻ�����
 * @param       saddr : ������ַ
 * @param       cnt   : ��������
 * @retval      0, ����;  ����, �������(���SD_Error����);
 */
uint8_t sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    uint8_t sta = HAL_OK;
    uint32_t timeout = SD_TIMEOUT_;
    long long lsector = saddr;
    __disable_irq();                                                           /* �ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!) */
    sta = HAL_SD_ReadBlocks(&hsd, (uint8_t *)pbuf, lsector, cnt, SD_TIMEOUT_); /* ���sector�Ķ����� */

    /* �ȴ�SD������ */
    while (get_sd_card_state() != SD_TRANSFER_OK)
    {
        if (timeout-- == 0)
        {
            sta = SD_TRANSFER_BUSY;
        }
    }
    __enable_irq(); /* �������ж� */
    
    return sta;
}

/**
 * @brief       дSD��(fatfs/usb����)
 * @param       pbuf  : ���ݻ�����
 * @param       saddr : ������ַ
 * @param       cnt   : ��������
 * @retval      0, ����;  ����, �������(���SD_Error����);
 */
uint8_t sd_write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    uint8_t sta = HAL_OK;
    uint32_t timeout = SD_TIMEOUT_;
    long long lsector = saddr;
    __disable_irq();                                                            /* �ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!) */
    sta = HAL_SD_WriteBlocks(&hsd, (uint8_t *)pbuf, lsector, cnt, SD_TIMEOUT_); /* ���sector��д���� */

    /* �ȴ�SD��д�� */
    while (get_sd_card_state() != SD_TRANSFER_OK)
    {
        if (timeout-- == 0)
        {
            sta = SD_TRANSFER_BUSY;
        }
    }
    __enable_irq();     /* �������ж� */
    
    return sta;
}
