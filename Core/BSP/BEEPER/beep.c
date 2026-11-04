#include "beep.h"

static uint8_t  buzzer_times;
static uint8_t  buzzer_phase;
static uint32_t buzzer_tick;

void Buzzer_Beep(uint8_t times)
{
    buzzer_times = times;
    buzzer_phase = 0;
    buzzer_tick  = HAL_GetTick();
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
}

void Buzzer_Update(void)
{
    if (buzzer_times == 0) return;

    uint32_t now = HAL_GetTick();
    if (buzzer_phase == 0) {
        if (now - buzzer_tick >= 100) {
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);
            buzzer_phase = 1;
            buzzer_tick  = now;
        }
    } else {
        if (now - buzzer_tick >= 100) {
            buzzer_times--;
            if (buzzer_times > 0) {
                HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
                buzzer_phase = 0;
                buzzer_tick  = now;
            }
        }
    }
}
