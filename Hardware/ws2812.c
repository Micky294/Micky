#include "ws2812.h"
#include "tim.h"

const Color_TypeDef BLACK  = {0,0,0};
const Color_TypeDef WHITE  = {255,255,255};
 
/*��ά����������PWM������飬ÿһ��24��
���ݴ���һ��LED�����һ��24��0����RESET��*/
uint32_t Pixel_Buffer[LED_Number+1][24];       
 
/*
���ܣ��趨����RGB LED����ɫ���ѽṹ����RGB��24BITת��Ϊ0���1��
������ledΪLED��ţ�Color���������ɫ�ṹ��
*/
void RGB_SetColor(uint8_t led,Color_TypeDef Color)
{
	uint8_t i; 
	if(led > LED_Number)
	return; 
	
	for(i=0;i<8;i++) Pixel_Buffer[led][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//����ĳһ��0~7ת�����G
	for(i=8;i<16;i++) Pixel_Buffer[led][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//����ĳһ��8~15ת�����R
	for(i=16;i<24;i++) Pixel_Buffer[led][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//����ĳһ��16~23ת�����B
}
 
/*
���ܣ����һ��װ��24��0�����24������ռ�ձ�Ϊ0��PWM������Ϊ���reset��ʱ��������ʱ��Ϊ24*1.2=30us > 24us(Ҫ�����24us)
*/
void Reset_Load(void)
{
	uint8_t i;
	for(i=0;i<24;i++)
	{
		Pixel_Buffer[LED_Number][i] = 0;
	}
}

/*
���ܣ���������
������(&htim1)��ʱ��5��(TIM_CHANNEL_2)ͨ��2��((uint32_t *)Pixel_Buffer)���������飬
			(Pixel_Num+1)*24)���͸����������������
*/
void RGB_SendArray(void)
{
	HAL_TIM_PWM_Start_DMA(&htim5, TIM_CHANNEL_2, (uint32_t *)Pixel_Buffer,(LED_Number+1)*24);
}
 
/*
���ܣ���ʾ��ɫ(����)
������LED����
*/
void RGB_WHITE(uint16_t LED_Num)
{
	uint16_t i;
	for(i=0;i<LED_Num;i++)
	{
		RGB_SetColor(i,WHITE);
	}
	Reset_Load();
	RGB_SendArray();
}

/*
���ܣ���ʾ��ɫ(����)
������LED����
*/
void RGB_BLACK(uint16_t LED_Num)
{
	uint16_t i;
	for(i=0;i<LED_Num;i++)
	{
		RGB_SetColor(i,BLACK);
	}
	Reset_Load();
	RGB_SendArray();
}
