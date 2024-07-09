#ifndef __WS2812_H__
#define __WS2812_H__
 
#include "main.h"
 
#define CODE_1       (58)       //1�붨ʱ����������
#define CODE_0       (25)       //0�붨ʱ����������
 
/*����LED��ԭɫֵ��С�Ľṹ��*/
typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}Color_TypeDef;
 
#define LED_Number 60  //LED����

void RGB_SetColor(uint8_t led,Color_TypeDef Color);
void Reset_Load(void); 
void RGB_SendArray(void);
void RGB_WHITE(uint16_t LED_Num);
void RGB_BLACK(uint16_t LED_Num);

#endif
