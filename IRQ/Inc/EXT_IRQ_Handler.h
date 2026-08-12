#ifndef EXT_IRQ_HANDLER_H
#define EXT_IRQ_HANDLER_H

#include "main.h"
#include "switch_sm.h"


void GPIO_EXTI_Callback(uint16_t GPIO_Pin,uint8_t* switch_state);

#endif

