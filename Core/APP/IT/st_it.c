#include "st_it.h"

volatile extern _Bool DHT_Flag;
volatile extern uint8_t soil_Flag;

extern TIM_HandleTypeDef htim2;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim2)
	{
		DHT_Flag=1;
		soil_Flag++;
	}

}
