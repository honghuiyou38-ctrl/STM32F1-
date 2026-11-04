#include "pump.h"
#include "pump_fsm.h"
#include "main.h"
#include "oled.h"
#include "usart.h"
#include "dht11.h"

#define HUMIUP       40
#define HUMIDOWN     25

static PumpMode_t pump_mode = PUMP_ST_AUTO;

extern uint8_t Message_Flag;
extern uint8_t soil_Humi;


void Pump_OFF(void)
{
	HAL_GPIO_WritePin(pump_GPIO_Port , pump_Pin, 1);
}


void Pump_ON(void)
{
	HAL_GPIO_WritePin(pump_GPIO_Port , pump_Pin, 0);
}


void Pump_OLED_sTate(void)
{
	static GPIO_PinState last_state;
	static uint8_t show_init = 0;
	GPIO_PinState current_state;

	current_state = HAL_GPIO_ReadPin(pump_GPIO_Port, pump_Pin);
	if ((show_init == 0) || (current_state != last_state))
	{
		OLED_ShowString(0, 7,(current_state == GPIO_PIN_SET) ? "PUMP:OFF" : "PUMP:ON ", 8);
		last_state = current_state;
		show_init = 1;
	}
}


void Pump_SetMode(PumpMode_t mode)
{
	pump_mode=mode;  //变量传输函数，代替extern，规范工程
}


void Plant_Pump_Task(void)
{
	 switch(pump_mode)
	 {
/*----------------------------状态机1--------------------------------*/
		 case PUMP_ST_AUTO:
		 {
			  if(Data[0]<90)
			  {
				  if(soil_Humi > HUMIUP)
				  {
					  Pump_OFF();//关闭水泵
				  }
				  else if(soil_Humi < HUMIDOWN)
				  {
					  Pump_ON();//开启水泵
				  }
			  }
			  else{Pump_OFF();}//湿度大于90判定下雨或潮湿锁死水泵

			  if(Message_Flag==1)
			  {
				  printf("Plant_Auto");
				  Message_Flag=0;
			  }
			  break;
		 }

/*----------------------------状态机2--------------------------------*/
		 case PUMP_ST_ON:
		 {
			  static uint32_t last_soil_print_tick = 0;
			  uint32_t now;

			  Pump_ON();

			  now = HAL_GetTick();
			  if ((now - last_soil_print_tick) >= 3000)//3秒打印一次土壤湿度
			  {
				  printf("soil_Humi: %u%%", soil_Humi);
				  last_soil_print_tick = now;
			  }
			  break;
		 }

/*----------------------------状态机3--------------------------------*/
		 case PUMP_ST_LOCK:
		 {
			  Pump_OFF();
			  if(Message_Flag==1)
			  {
				  printf("LOCK PUMP");
				  Message_Flag=0;
			  }
			  break;
		 }

/*----------------------------防错误跳转--------------------------------*/
		 default:
		 {
			 pump_mode=PUMP_ST_LOCK;
			 Pump_OFF();
			 break;
		 }
	 }

}

