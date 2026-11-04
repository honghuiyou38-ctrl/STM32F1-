#ifndef __FAN_H_
#define __FAN_H_

#include "fan_fsm.h"
#include "motor.h"
#include "stm32f1xx.h"
#include "string.h"
#include "stdio.h"
#include "oled.h"

extern uint8_t Fan_Value;

void Plant_Fan_APP(void);
void Fan_SetMode(FanMode_t mode);

/*----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----*/
void Plant_Fan_Task(void);
/*----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----*/


#endif
