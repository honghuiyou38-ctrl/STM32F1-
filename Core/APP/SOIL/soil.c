#include "soil.h"
#include "stm32f1xx.h"
#include "string.h"
#include "stdio.h"

#define OLD_WEIGHT   0.7
#define NEW_WEIGHT   0.3

extern uint32_t adc_soil;
extern uint8_t soil_Humi;
extern uint8_t soil_Flag;
extern unsigned char Soil_buf[20];


extern ADC_HandleTypeDef hadc1;

void soil_Humi_Take(void) //土壤湿度采集
{
	static uint8_t soil_Humi_last=0;
	uint8_t soil_Humi_now;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	adc_soil=HAL_ADC_GetValue(&hadc1);

	soil_Humi_now=(4096-adc_soil)*100/4096; //ADC采样值百分比转化湿度
	if(soil_Humi_last==0)
	{
		soil_Humi=soil_Humi_now;
	}
	else
	{
		soil_Humi = OLD_WEIGHT * soil_Humi_last + NEW_WEIGHT * soil_Humi_now; //一阶低通滤波
	}

	soil_Humi_last=soil_Humi;

	sprintf(Soil_buf,"SOIL:%u%%", soil_Humi);

	soil_Flag=0;
}
