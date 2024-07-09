#ifndef __WS2812_H__
#define __WS2812_H__
 
#include "main.h"
 
#define CODE_1       (58)       //1码定时器计数次数
#define CODE_0       (25)       //0码定时器计数次数
 
/*单个LED三原色值大小的结构体*/
typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}Color_TypeDef;
 
#define LED_Number 60  //LED数量

void RGB_SetColor(uint8_t led,Color_TypeDef Color);
void Reset_Load(void); 
void RGB_SendArray(void);
void RGB_WHITE(uint16_t LED_Num);
void RGB_BLACK(uint16_t LED_Num);

#endif
