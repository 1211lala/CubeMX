#ifndef __WS2812_H__
#define __WS2812_H__

#include "main.h"



#define 		WS_OFF					0x000000
#define     WS_WHITE        0xFFFFFF
#define     WS_RED          0XFF0000
#define     WS_RGREEN       0X00FF00
#define     WS_BLUE         0X0000FF

#define     WS_ORANGE       0XF28500     /* ��ɫ */

#define     WS_IvyGreen     0X36BF36    /* �������� */
#define     ForestGreen     0X228B22    /* ɭ���� */
#define     LimeGreen       0X32CD32    /* ������ */
#define     DarkGreen       0X006400    /* ���� */
#define     Lime            0X00FF00    /* ����ɫ */
#define     Malachite       0X22C32E    /* ��ȸʯ�� */


#define     SalviaBlue      0X4D80E6    /* β���� */
#define     DarkPowder      0X003399    /* ��Ӥ������ */
#define     Sapphire        0X082567    /* ����ʯɫ */
#define     International   0X002FA7    /* ���������� */
#define     Ceruleanblue    0X2A52BE    /* ε�� */
#define     RoyalBlue       0X4169E1    /* Ʒ�� */
#define     DarkMineralBlue 0X24367D    /* ������ */
#define     Ultramarine     0X0033FF    /* ��Ũ���� */
#define     LapisLazuli     0X0D33FF    /* ����ʯ�� */



#define     Purple              0X800080    /* ��ɫ */
#define     DarkMagenta         0X8B008B    /* ����� */
#define     Magenta             0XFF00FF    /* ��� */
#define     Fuchsia             0XF400A1    /* Ʒ�� */
#define     Orchid              0XDA70D6    /* ���� */
#define     PearlPink           0XFFB3E6    /* ǳ����� */
#define     OldRose             0XB85798    /* ��õ�� */
#define     RosePink            0XFF66CC    /* ǳõ��� */
#define     MediumVioletRed     0XC71585    /* �����Ϻ� */
#define     MagentaRose         0XFF0DA6    /* ��õ��� */
#define     Rose                0XFF007F    /* õ��� */
#define     Ruby                0XCC0080    /* �챦ʯɫ */
#define     Camellia            0XE63995    /* ɽ��� */
#define     DeepPink            0XFF1493    /* ��ۺ� */
#define     Flamingo            0XE68AB8    /* ��׺� */
#define     CoralPink           0XFF80BF    /* ǳɺ���� */


#define 	WS2812_NUM           30

#define 	CODE_1               49       	       
#define 	CODE_0               23 


#define 	WS2812_TIM           &htim1            
#define   WS2812_CHANNEL     	 TIM_CHANNEL_3



void WS2812_DMA_Send_buf(void);
void WS2812_Set_RGB_Color(uint16_t ID,uint32_t Color);
void WS2812_Set_R_G_B_Color(uint16_t ID, uint8_t r, uint8_t g, uint8_t b);
void WS2812_Reset(void);
void WS2812_Refresh(void);
void WS2812_Clear(uint32_t color);
void WS2812_Random_Display(uint16_t time);
void WS2812_Random_Point_Flow(uint16_t time);
void WS2812_Random_Line_Flow(uint16_t time);


uint16_t rgb888_to_rgb565(uint32_t rgb888);
uint32_t rgb565_to_rgb888(uint16_t rgb565);

//void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);  /*��ɫ�㷨 RGB-> hsv*/
//void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);  /*��ɫ�㷨 hsv-> RGB*/
//RGB_Color_TypeDef WS2812_SetBrightness(float num, RGB_Color_TypeDef rgb);

#endif
