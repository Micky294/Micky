#include "DHT11.h"
#include "tim.h"
#include "gpio.h"
 
void delay_us(uint16_t us)
{     //微秒延时
	uint16_t differ = 0xffff-us-5;				
	__HAL_TIM_SET_COUNTER(&htim2,differ);	//设定TIM1计数器起始值
	HAL_TIM_Base_Start(&htim2);		//启动定时器	
	
	while(differ < 0xffff-5){	//判断
		differ = __HAL_TIM_GET_COUNTER(&htim2);		//查询计数器的计数值
	}
	HAL_TIM_Base_Stop(&htim2);
}

void DHT_GPIO_Init(uint32_t Mode)
{
 	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = Temp_Pin;
	GPIO_InitStruct.Mode = Mode;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/**
  * @brief  DHT11模块起始信号函数
  * @param  None
  * @return 1或0，标志起动信号成功与否
  */

uint8_t DHT_Start(void)
{
	DHT_GPIO_Init(GPIO_MODE_OUTPUT_PP);							//输出模式
	
	HAL_GPIO_WritePin(GPIOE,Temp_Pin,GPIO_PIN_RESET);		//输出30ms低电平后拉高
	HAL_Delay(30);
	HAL_GPIO_WritePin(GPIOE,Temp_Pin,GPIO_PIN_SET);
	
	DHT_GPIO_Init(GPIO_MODE_INPUT);					//输入模式
	delay_us(20);
	
	if(!HAL_GPIO_ReadPin(GPIOE,Temp_Pin))
	{
		while(!HAL_GPIO_ReadPin(GPIOE,Temp_Pin));
		while(HAL_GPIO_ReadPin(GPIOE,Temp_Pin));
		return 1;
	}
	return 0;
}

/**
  * @brief  接收DHT11发送来8位的数据
  * @param  None
  * @return 返回接收到的8位数据
  */

uint8_t DHT_Get_Byte_Data(void)
{
	uint8_t Temp;
	for(int i = 0; i < 8; i++)
	{
		Temp <<= 1;
		while(!HAL_GPIO_ReadPin(GPIOE,Temp_Pin));
		delay_us(28);
		
		HAL_GPIO_ReadPin(GPIOE,Temp_Pin) ? (Temp |= 0x01) : (Temp &= ~0x01);
		while(HAL_GPIO_ReadPin(GPIOE,Temp_Pin));
	}
	return Temp;
}

/**
  * @brief  获取DHT11的温度湿度数据
  * @param  buffer[]：需要传入一个存储数据的数组
  * @return 返回数据校验是否正确 1：正确 0：失败
  */
uint8_t DHT_Get_Temp_Humi_Data(uint8_t buffer[])
{
	if(DHT_Start())
	{
		buffer[0] = DHT_Get_Byte_Data();
		buffer[1] = DHT_Get_Byte_Data();
		buffer[2] = DHT_Get_Byte_Data();
		buffer[3] = DHT_Get_Byte_Data();
		buffer[4] = DHT_Get_Byte_Data();
	}
	return (buffer[0]+buffer[1]+buffer[2]+buffer[3] == buffer[4]) ? 1 : 0;
}
