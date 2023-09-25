#include ".\WS2812\ws2812.h"
 
 
uint32_t WS2812_Buf[WS2812_NUM+1][24];   	        /*���ݴ洢����*/

/**
 * @brief  �ڻ������ݸ��º�ʹ�ô˺���ˢ����ʾ
 */
void WS2812_DMA_Send_buf(void)
{
	HAL_TIM_PWM_Start_DMA(WS2812_TIM, WS2812_CHANNEL, (uint32_t *)WS2812_Buf, (WS2812_NUM+1)*24);
}


/**
 * @brief   ��WS2812��ɫ����д�뻺��
 * @param   ID    WS2812�����
 * @param   Color Ҫд����ɫ
 * @retval  ��
 */
void WS2812_Set_R_G_B_Color(uint16_t ID, uint8_t r, uint8_t g, uint8_t b)
{
	if(ID > WS2812_NUM)return; 
	
	for(uint8_t i =0; i<8; i++)	/* д��g���� */
	{
		if(g & (1<<(7-i)))
		{
			WS2812_Buf[ID][i] = CODE_1;
		}
		else
		{
			WS2812_Buf[ID][i] = CODE_0;
		} 
	}
	for(uint8_t i =8; i<16; i++)	/* д��r���� */
	{
		if(r & (1<<(15-i)))
		{
			WS2812_Buf[ID][i] = CODE_1;
		}
		else
		{
			WS2812_Buf[ID][i] = CODE_0;
		}
	}
	for(uint8_t i =16; i<24; i++)	/* д��b���� */
	{
		if(b & (1<<(23-i)))
		{
			WS2812_Buf[ID][i] = CODE_1;
		}
		else
		{
			WS2812_Buf[ID][i] = CODE_0;
		}
	}
}
 
/**
 * @brief   ��WS2812��ɫ����д�뻺��
 * @param   ID    WS2812�����
 * @param   r g b Ҫд����ɫ
 * @retval  ��
 */
void WS2812_Set_RGB_Color(uint16_t ID, uint32_t Color)
{
	uint8_t r = Color >>16;
	uint8_t g = Color >>8;
	uint8_t b = Color >>0;
	WS2812_Set_R_G_B_Color(ID, r, g, b);
}


/**
 * @brief ��WS2812��reset�ź� > 24us
 */
void WS2812_Reset(void)
{
	uint8_t i;
	for(i=0;i<24;i++)
	{
		WS2812_Buf[WS2812_NUM][i] = 0;
	}
}
/**
* @brief �������е����ݷ��ͳ�ȥ
 */
void WS2812_Refresh(void)
{
	WS2812_Reset();
	WS2812_DMA_Send_buf();
}

 






 




//static float min(float a, float b, float c)
//{
//  float m;
//  
//  m = a < b ? a : b;
//  return (m < c ? m : c); 
//}

//static float max(float a, float b, float c)
//{
//  float m;
//  
//  m = a > b ? a : b;
//  return (m > c ? m : c); 
//}
//  
//void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
//{
//  float red, green ,blue;
//  float cmax, cmin, delta;
//  
//  red = (float)r / 255;
//  green = (float)g / 255;
//  blue = (float)b / 255;
//  
//  cmax = max(red, green, blue);
//  cmin = min(red, green, blue);
//  delta = cmax - cmin;

//  /* H */
//  if(delta == 0)
//  {
//    *h = 0;
//  }
//  else
//  {
//    if(cmax == red)
//    {
//      if(green >= blue)
//      {
//        *h = 60 * ((green - blue) / delta);
//      }
//      else
//      {
//        *h = 60 * ((green - blue) / delta) + 360;
//      }
//    }
//    else if(cmax == green)
//    {
//      *h = 60 * ((blue - red) / delta + 2);
//    }
//    else if(cmax == blue) 
//    {
//      *h = 60 * ((red - green) / delta + 4);
//    }
//  }
//  
//  /* S */
//  if(cmax == 0)
//  {
//    *s = 0;
//  }
//  else
//  {
//    *s = delta / cmax;
//  }
//  
//  /* V */
//  *v = cmax;
//}
//  
//void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
//{
//    int hi = ((int)h / 60) % 6;
//    float f = h * 1.0 / 60 - hi;
//    float p = v * (1 - s);
//    float q = v * (1 - f * s);
//    float t = v * (1- (1 - f) * s);
//    switch (hi){
//        case 0:
//            *r = 255 * v;
//            *g = 255 * t;
//            *b = 255 * p;
//            break;
//        case 1:
//            *r = 255 * q;
//            *g = 255 * v;
//            *b = 255 * p;
//            break;
//        case 2:
//            *r = 255 * p;
//            *g = 255 * v;
//            *b = 255 * t;
//            break;
//        case 3:
//            *r = 255 * p;
//            *g = 255 * q;
//            *b = 255 * v;
//            break;
//        case 4:
//            *r = 255 * t;
//            *g = 255 * p;
//            *b = 255 * v;
//            break;
//        case 5:
//            *r = 255 * v;
//            *g = 255 * p;
//            *b = 255 * q;
//            break;
//    }
//}



//RGB_Color_TypeDef WS2812_SetBrightness(float num, RGB_Color_TypeDef rgb)
//{
//  float h, s,v;
//  rgb2hsv(rgb.R, rgb.G, rgb.B, &h, &s, &v);
//  v=num;
//  hsv2rgb(h, s, v, &rgb.R, &rgb.G, &rgb.B);
//  return rgb;
//}
