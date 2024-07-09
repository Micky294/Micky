#include "DHT11.h"
#include "tim.h"
#include "gpio.h"
 
void delay_us(uint16_t us)
{     //΢����ʱ
	uint16_t differ = 0xffff-us-5;				
	__HAL_TIM_SET_COUNTER(&htim2,differ);	//�趨TIM1��������ʼֵ
	HAL_TIM_Base_Start(&htim2);		//������ʱ��	
	
	while(differ < 0xffff-5){	//�ж�
		differ = __HAL_TIM_GET_COUNTER(&htim2);		//��ѯ�������ļ���ֵ
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
  * @brief  DHT11ģ����ʼ�źź���
  * @param  None
  * @return 1��0����־���źųɹ����
  */

uint8_t DHT_Start(void)
{
	DHT_GPIO_Init(GPIO_MODE_OUTPUT_PP);							//���ģʽ
	
	HAL_GPIO_WritePin(GPIOE,Temp_Pin,GPIO_PIN_RESET);		//���30ms�͵�ƽ������
	HAL_Delay(30);
	HAL_GPIO_WritePin(GPIOE,Temp_Pin,GPIO_PIN_SET);
	
	DHT_GPIO_Init(GPIO_MODE_INPUT);					//����ģʽ
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
  * @brief  ����DHT11������8λ������
  * @param  None
  * @return ���ؽ��յ���8λ����
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
  * @brief  ��ȡDHT11���¶�ʪ������
  * @param  buffer[]����Ҫ����һ���洢���ݵ�����
  * @return ��������У���Ƿ���ȷ 1����ȷ 0��ʧ��
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
