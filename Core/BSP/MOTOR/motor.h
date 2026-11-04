#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx.h"
#include "string.h"
#include "stdio.h"
#include "oled.h"

void Motor_Init(void);//初始化电机
void Motor_ON(void);
void Motor_OFF(void);
void Motor_Gears(unsigned int value);

#endif
