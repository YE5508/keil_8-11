/**
 * @file    timer.h
 * @brief   timer头文件。
 *          头文件放声明和宏，具体实现放在 timer.c。
 */
#ifndef TIMER_H
#define TIMER_H

#include "main.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 宏定义 */




typedef struct timer
{
     uint32_t start_time;
     uint32_t duration;
     uint8_t running;
    
}timer;


void timer_start(timer* timer);
bool timer_is_expired (timer* timer,uint8_t duration);
#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */
