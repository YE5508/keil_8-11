/**
 * @file    buzzer.c
 * @brief   蜂鸣器驱动实现文件。
 */
#include "buzzer.h"
#include "TIM_IRQ_Handler.h"

/* 初始化：先把蜂鸣器引脚设置为关闭状态 */
void buzzer_init(void)
{
    buzzer_off();
}

/* 打开蜂鸣器：把 PB0 输出高电平 */
void buzzer_on(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_SET);
}

/* 关闭蜂鸣器：把 PB0 输出低电平 */
void buzzer_off(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

void buzzer_toggle(void)
{
    HAL_GPIO_TogglePin(BUZZER_GPIO_PORT, BUZZER_PIN);
}

void Beep(void)
{
    buzzer_on();
    HAL_Delay(50);
    buzzer_off();
    HAL_Delay(100);

}

void Beep_Alarm(uint8_t beep_times)
{
    for(int i =0;i<beep_times;i++)
    {
        Beep();
    }
    
}
