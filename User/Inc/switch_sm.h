#ifndef __SWITCH_SM_H
#define __SWITCH_SM_H
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))
#define PWM_ARR 1000
/*include*/

#include "led.h"
#include "buzzer.h"
#include "timer.h"
#include "TIM_IRQ_Handler.h"

/*PD*/
typedef void(* state_func)(void);
typedef enum
{
    IDLE_STATE = 0,
    SW_STATE,
    FLOW_STATE,
    BREATH_STATE,
    SWITCH_SM_STATE_COUNT
}SWITCH_SM_STATE;

/*PF*/
void idle_state(void);
void sw_state(void);
void flow_state(void);
void breath_state(void);
void state_run(uint8_t state);

/*PV*/
extern const state_func switch_SM[];
extern const uint16_t flow_state_table[];

#define FLOW_STATE_COUNT ARRAY_SIZE(flow_state_table)


#endif