#ifndef UART_IRQ_HANDLER_H
#define UART_IRQ_HANDLER_H
#define ARRLEN(arr) sizeof(arr)/sizeof((arr)[0]) 

#include "main.h"//内含HAL库
#include "usart.h"
#include "string.h"

extern uint8_t beep_times;
extern uint8_t tx_flag;
extern uint8_t tx_buffer[100];

void UART_Start_Receive(void);

#define BUFFER_LEN ARRLEN(tx_buffer)

#endif

