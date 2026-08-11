#ifndef __TIM_IRQ_HANDLER_H
#define __TIM_IRQ_HANDLER_H



#include "stm32f4xx_hal.h"
#include "tim.h"

extern volatile uint8_t tim2out_flag;

void TIM2_start(void);
void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif