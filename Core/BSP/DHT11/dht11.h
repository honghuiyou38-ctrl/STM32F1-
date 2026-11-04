#ifndef __DHT11_H
#define __DHT11_H

/*------------头文件--------------头文件----------------头文件-------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
/*------------头文件--------------头文件----------------头文件-------*/


/*---------PV-------PV------PV--PV--PV--PV---*/
extern uint8_t Data[5];
extern TIM_HandleTypeDef htim1;
/*-----------PV--------PV--PV--PV--PV--PV--*/


/*------------函数--------------函数----------------函数-------*/
void Delay_us(uint16_t us);
void DHT_GPIO_SET_OUTPUT(void);
void DHT_GPIO_SET_INPUT(void);
uint8_t DHT_Read_Byte(void);
uint8_t DHT_Read(void);
/*------------函数--------------函数----------------函数-------*/


#endif
