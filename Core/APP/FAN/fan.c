#include "fan.h"
#include "fan_fsm.h"
#include "dht11.h"
#include "usart.h"

#define TEMP_SET         27.0f
#define KP               0.7f

extern TIM_HandleTypeDef htim3;

static FanMode_t fan_mode=FAN_ST_AUTO;

uint8_t Fan_Value=0;
uint8_t Fan_Flag=0;
extern uint8_t Message_Flag_FAN;


unsigned char Fan_buf[20];

void Plant_Fan_APP(void)
{
	float temp_error;
	float temp_out;

	temp_error=Data[2]-TEMP_SET;
	temp_out=KP*temp_error;

	if(temp_out<=0)
	{
		Fan_Value=0;
	}
	else if(temp_out>=10)
	{
		Fan_Value=10;
	}
	else
	{
		Fan_Value=temp_out;
	}

	Motor_Gears(Fan_Value);
	sprintf(Fan_buf," FAN_%u ", Fan_Value);
	OLED_ShowString(64,4,Fan_buf,16);

}

void Fan_SetMode(FanMode_t mode)
{
	fan_mode=mode;
}


void Plant_Fan_Task(void)
{

	switch(fan_mode)
	{
/*----------------------------状态机1--------------------------------*/
		case FAN_ST_AUTO:
		{
			Plant_Fan_APP();
			if(Message_Flag_FAN==1)
			{
				printf("Auto fan");
				Message_Flag_FAN=0;
			}
			break;
		}

/*----------------------------状态机2--------------------------------*/
		case FAN_ST_LOCK:
		{
			Motor_OFF();
			OLED_ShowString(64,4,"FAN---",16);
			if(Message_Flag_FAN==2)
			{
				printf("fan OFF");
				Message_Flag_FAN=0;
			}
			break;
		}

/*----------------------------状态机3--------------------------------*/
		case FAN_ST_ON:
		{
			Motor_ON();
			OLED_ShowString(64,4,"FAN ON",16);
			Motor_Gears(3);
			if(Message_Flag_FAN==3)
			{
				printf("fan ON");
				Message_Flag_FAN=0;
			}
			break;
		}

/*----------------------------防错误跳转--------------------------------*/
		default:
			fan_mode=FAN_ST_LOCK;
			Motor_OFF();
			break;

	}

}
