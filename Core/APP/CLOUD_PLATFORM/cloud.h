#ifndef __CLOUD_H
#define __CLOUD_H

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "oled.h"
#include "beep.h"

#define UART_SIZE_RECV    128

extern volatile uint8_t Get_Value_Flag;

void Data_Form_Print(void);

#endif
