#include "switch_sm.h"
#define FLOW_INTERVAL_MS 200U

uint8_t TIM2_start_flag = 0;

const state_func switch_SM[]=
{
    [IDLE_STATE] = idle_state,
    [SW_STATE] = sw_state,
    [FLOW_STATE] = flow_state,
    [BREATH_STATE] = breath_state
};


static void flow_off(void)
{
    led_off(1);
    led_off(3);
}

void idle_state(void)
{
    //buzzer_off();
    flow_off();
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
    static const uint8_t led_table[]={1,3};
    static uint8_t led_index =0;
    static uint32_t last_time = 0;
    if(HAL_GetTick()-last_time >FLOW_INTERVAL_MS )
    {
        last_time = HAL_GetTick();
        led_off(1);
        led_off(3);
        led_on(led_table[led_index]);
        led_index=(led_index+1)%2;
    }

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
            switch_SM[FLOW_STATE]();
            break;
        case BREATH_STATE:
            switch_SM[BREATH_STATE]();
            flow_off();
            break;
    }
    



}
