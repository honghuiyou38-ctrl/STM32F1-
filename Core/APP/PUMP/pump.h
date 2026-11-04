#ifndef _PUMP_H
#define _PUMP_H

#include "string.h"
#include "stdio.h"
#include "cloud.h"
#include "pump_fsm.h"

void Pump_OFF(void);
void Pump_ON(void);
void Pump_OLED_sTate(void);

void Pump_SetMode(PumpMode_t mode);

/*----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----*/
void Plant_Pump_Task(void);
/*----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----水泵任务逻辑----*/

#endif
