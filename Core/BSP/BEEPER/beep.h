#ifndef __BEEP_H
#define __BEEP_H

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "oled.h"

void Buzzer_Beep(uint8_t times);
void Buzzer_Update(void);

#endif
