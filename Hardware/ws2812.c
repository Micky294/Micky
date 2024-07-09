#include "ws2812.h"
#include "tim.h"

const Color_TypeDef BLACK  = {0,0,0};
const Color_TypeDef WHITE  = {255,255,255};
 
/*二维数组存放最终PWM输出数组，每一行24个
数据代表一个LED，最后一行24个0代表RESET码*/
uint32_t Pixel_Buffer[LED_Number+1][24];       
 
/*
功能：设定单个RGB LED的颜色，把结构体中RGB的24BIT转换为0码和1码
参数：led为LED序号，Color：定义的颜色结构体
*/
void RGB_SetColor(uint8_t led,Color_TypeDef Color)
{
	uint8_t i; 
	if(led > LED_Number)
	return; 
	
	for(i=0;i<8;i++) Pixel_Buffer[led][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
	for(i=8;i<16;i++) Pixel_Buffer[led][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
	for(i=16;i<24;i++) Pixel_Buffer[led][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}
 
/*
功能：最后一行装在24个0，输出24个周期占空比为0的PWM波，作为最后reset延时，这里总时长为24*1.2=30us > 24us(要求大于24us)
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
功能：发送数组
参数：(&htim1)定时器5，(TIM_CHANNEL_2)通道2，((uint32_t *)Pixel_Buffer)待发送数组，
			(Pixel_Num+1)*24)发送个数，数组行列相乘
*/
void RGB_SendArray(void)
{
	HAL_TIM_PWM_Start_DMA(&htim5, TIM_CHANNEL_2, (uint32_t *)Pixel_Buffer,(LED_Number+1)*24);
}
 
/*
功能：显示白色(灯亮)
参数：LED个数
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
功能：显示黑色(灯灭)
参数：LED个数
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
