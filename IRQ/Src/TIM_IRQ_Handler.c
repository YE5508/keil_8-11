#include "TIM_IRQ_Handler.h"
#include "led.h"

volatile uint8_t tim2out_flag = 1;
volatile uint16_t buzzer_time = 0;

void TIM2_start(void)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim2);
}

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)//TIM2用于非阻塞式蜂鸣器
    {
        HAL_TIM_Base_Stop_IT(htim);
        // 计时结束后的处理
        buzzer_off();

    }
}
