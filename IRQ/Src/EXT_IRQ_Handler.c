#include "EXT_IRQ_Handler.h"

void GPIO_EXTI_Callback(uint16_t GPIO_Pin,uint8_t* switch_state)
{
      static uint32_t last_time=0;
  switch(GPIO_Pin)
  {
    case GPIO_PIN_11:
      switch(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11))
      {
        case 1:
          last_time = HAL_GetTick();
          switch_state = SW_STATE;
          break;
        case 0:
          if(HAL_GetTick()-last_time<1000)
          {
            switch_state = FLOW_STATE;
            
          }
          else
          {
          switch_state = BREATH_STATE;            
          }

          break;

      }
      
      break;
  }
}
