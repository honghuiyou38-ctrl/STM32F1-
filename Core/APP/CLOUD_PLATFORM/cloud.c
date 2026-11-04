#include "cloud.h"
#include "pump.h"
#include "fan.h"
#include "dht11.h"
#include "usart.h"

extern uint8_t UART_DMAbuf[UART_SIZE_RECV];

extern uint8_t soil_Humi;
volatile uint8_t Message_Flag=0;
volatile uint8_t Message_Flag_FAN=0;
volatile uint8_t Get_Value_Flag=0;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart==&huart1)
	{

		if(strcmp((char *)UART_DMAbuf,"PumpON")==0)//开启水泵
		{
			Pump_SetMode(PUMP_ST_ON);
			memset(UART_DMAbuf,'\0',Size);
			Buzzer_Beep(1);
		}
		else if(strcmp((char *)UART_DMAbuf,"PumpOFF")==0)//关闭水泵
		{
			Pump_SetMode(PUMP_ST_LOCK);
			Message_Flag=1;
			memset(UART_DMAbuf,'\0',Size);
			Buzzer_Beep(1);
		}
		else if(strcmp((char *)UART_DMAbuf,"PumpAuto")==0)//自动灌溉
		{
			Pump_SetMode(PUMP_ST_AUTO);
			Message_Flag=1;
			memset(UART_DMAbuf,'\0',Size);
			Buzzer_Beep(1);
		}

/*------------------------------------------------------------------------------------------------------------------*/

		if(strcmp((char *)UART_DMAbuf,"FANAuto")==0)//自动风扇调节
		{
			Fan_SetMode(FAN_ST_AUTO);
			Message_Flag_FAN=1;
			memset(UART_DMAbuf,'\0',Size);
			Buzzer_Beep(1);
		}
		else if(strcmp((char *)UART_DMAbuf,"FANOFF")==0)//风扇关闭锁死
		{
			Fan_SetMode(FAN_ST_LOCK);
			Message_Flag_FAN=2;
			memset(UART_DMAbuf,'\0',Size);
			Buzzer_Beep(1);
		}
		else if(strcmp((char *)UART_DMAbuf,"FANON")==0)//风扇开启
		{
			Fan_SetMode(FAN_ST_ON);
			Message_Flag_FAN=3;
			memset(UART_DMAbuf,'\0',Size);
			Buzzer_Beep(1);
		}

/*------------------------------------------------------------------------------------------------------------------*/

		else if(strcmp((char *)UART_DMAbuf,"GETData")==0)
		{
			Get_Value_Flag=1;
			memset(UART_DMAbuf,'\0',Size);
			Buzzer_Beep(1);
		}

		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, UART_DMAbuf, UART_SIZE_RECV - 1U);
	}
}

//表格输出数据：

void Data_Form_Print(void)
{
	if(Get_Value_Flag==1)
	{
		printf("\n|-----------------------|\n");
		printf("|  -----Plant data-----  |\n");
		printf("|-----------------------|\n");
		printf("|  %-12s|   %2u%%   |\n","AIR_Humi", Data[0]);
		printf("|-----------------------|\n");
		printf("|  %-12s|    %2uC   |\n","AIR_Temp", Data[2]);
		printf("|-----------------------|\n");
		printf("| %-12s|    %2u%%   |\n","SOIL_Humi", soil_Humi);
		printf("|-----------------------|\n");
		Get_Value_Flag=0;
	}
}
