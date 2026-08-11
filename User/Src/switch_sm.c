#include "switch_sm.h"


uint8_t TIM2_start_flag = 0;

const state_func switch_SM[]=
{
    [IDLE_STATE] = idle_state,
    [SW_STATE] = sw_state,
    [FLOW_STATE] = flow_state,
    [BREATH_STATE] = breath_state
};

const uint16_t flow_state_table[]=
{
    LED3_PIN,
    LED3_PIN,
    LED1_PIN,
    LED1_PIN,
};

static void flow_off(void)
{
    led_off(1);
    led_off(3);
}

void idle_state(void)
{
    buzzer_off();
    for(uint8_t i = 1;i<5;i++)
    {
        led_off(i);
    }
}

void sw_state(void)
{   
    static timer timer= {0,0,0};
    if(!timer.running)
    {
    timer_start(&timer);
    }

    if(timer_is_expired(&timer,200))
    {
        buzzer_toggle();
        timer_start(&timer);
    }
    
}
void flow_state(void)
{
    buzzer_off();
    static uint8_t flowstate = 0;
    led_toggle(flow_state_table[(flowstate ++)%FLOW_STATE_COUNT]);
}



void breath_state(void)
{
    buzzer_off();
    int pwmVAL=0;
    while(pwmVAL<PWM_ARR)
    {
        /* code */
        pwmVAL+=1;
        __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,pwmVAL);
        __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,pwmVAL);
        HAL_Delay(1);

    }
   while (pwmVAL > 0)
    {

        pwmVAL-=1;
        __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,pwmVAL);
        __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,pwmVAL);
        HAL_Delay(1);

    }
    
    
}

void state_run(uint8_t state)
{   
    if(state>=SWITCH_SM_STATE_COUNT || switch_SM[state]==NULL)
    {
        return;
    }

    switch (state)
    {
        case IDLE_STATE:
            switch_SM[IDLE_STATE]();
            break;
        case SW_STATE:
            switch_SM[SW_STATE]();
            flow_off();
            break;
        case FLOW_STATE:
        if(!TIM2_start_flag)
        {
            TIM2_start();   
            TIM2_start_flag=1;    
        }

            if(tim2out_flag)
            {
                flow_state();
                tim2out_flag = 0;
                TIM2_start_flag = 0;
            }
            break;
        case BREATH_STATE:
            switch_SM[BREATH_STATE]();
            flow_off();
            break;
    }
    



}