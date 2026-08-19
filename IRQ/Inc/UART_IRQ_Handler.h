#ifndef UART_IRQ_HANDLER_H
#define UART_IRQ_HANDLER_H
#define ARRLEN(arr) sizeof(arr)/sizeof((arr)[0]) 
#define CH_COUNT 1

#include "main.h"//内含HAL库
#include "usart.h"
#include "string.h"

extern volatile uint8_t Beep_Trigger;
extern uint8_t tx_flag;
extern uint8_t tx_buffer[100];

typedef struct Frame
{
    float fdata[CH_COUNT];
    uint8_t tail[4];
}Frame;

void UART_Start_Receive(void);
void UART_Send_Float(float data);

#define BUFFER_LEN ARRLEN(tx_buffer)

#endif
