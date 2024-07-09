#include "main.h"
#include "tim.h"
#include "gpio.h"

void motor_Open(void)  //电机正转
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	
	HAL_GPIO_WritePin(GPIOB,AIN1_Pin|BIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,AIN2_Pin|BIN2_Pin,GPIO_PIN_RESET);
	
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,280);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,280);
}

void motor_Close(void)  //电机反转
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	
	HAL_GPIO_WritePin(GPIOB,AIN1_Pin|BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,AIN2_Pin|BIN2_Pin,GPIO_PIN_SET);
	
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,280);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,280);}

void motor_Stop(void)  //电机停止
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

	HAL_GPIO_WritePin(GPIOB,AIN1_Pin|BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,AIN2_Pin|BIN2_Pin,GPIO_PIN_RESET);
	
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,280);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,280);}
