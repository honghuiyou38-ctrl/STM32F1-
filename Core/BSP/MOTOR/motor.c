#include "motor.h"

extern TIM_HandleTypeDef htim3;

void Motor_Init(void)//初始化电机
{
	HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, 0);
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 0);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 0);
}


void Motor_ON(void)
{
	HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, 1);
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 1);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 0);
}

void Motor_OFF(void)
{
	HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, 1);
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 0);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 0);
}

void Motor_Gears(unsigned int value) //电机1--10档
{
	Motor_ON();

	if(value>10)
	value=10;
							//限幅
	else if(value<0)
	value=0;

	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,10*value);
}




