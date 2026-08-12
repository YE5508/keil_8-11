/**
 * @file    buzzer.h
 * @brief   蜂鸣器驱动头文件。
 *          头文件放声明和宏，具体实现放在 buzzer.c。
 */
#ifndef BUZZER_H
#define BUZZER_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 宏定义：给蜂鸣器使用的端口和引脚起一个容易记住的名字 */
#define BUZZER_GPIO_PORT BEEP_GPIO_Port
#define BUZZER_PIN       BEEP_Pin
#define BEEP_TIME   100

#include "timer.h"
#include "TIM_IRQ_Handler.h"

/* 函数声明（原型）：只声明函数存在，不写实现 */
void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_toggle(void);
void Beep(void);
void Beep_Alarm(uint8_t beep_times);

#ifdef __cplusplus
}
#endif

#endif /* BUZZER_H */