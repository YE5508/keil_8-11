#include "UART_IRQ_Handler.h"
#include "main.h"

uint8_t rx_buffer[100] = {0};
uint8_t volatile Beep_Trigger = 0;
uint8_t tx_buffer[100] = {0};
uint8_t tx_flag = 0;

void UART_Start_Receive(void)
{
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,rx_buffer,sizeof(rx_buffer));
    //HAL_UART_Receive_IT(&huart1, rx_buffer, 5); // UART接收-定长中断
    //HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rx_buffer,sizeof(rx_buffer));
    //HAL_UART_Receive_DMA(&huart1, rx_buffer, 5);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (rx_buffer[0] == 0xff)
        {
            for (int i = 1; i < 5; i++)
            {
                if (rx_buffer[i] == 1)
                {
                    Beep_Trigger++;
                }
            }
        }
        memcpy(tx_buffer, rx_buffer,BUFFER_LEN);
        tx_flag = 1; // tx_flag = 1则开启串口发送
        HAL_UART_Transmit_DMA(&huart1,tx_buffer,5);
        HAL_UART_Receive_DMA(&huart1, rx_buffer, 5);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        if (rx_buffer[0] == 0xff)
        {
            for (int i = 1; i < Size; i++)
            {
                if (rx_buffer[i] == 1)
                {
                    Beep_Trigger++;
                }
            }
        }
        memcpy(tx_buffer, rx_buffer, Size);
        tx_flag = 1; // tx_flag = 1则开启串口发送
        HAL_UART_Transmit_DMA(&huart1,tx_buffer,Size);

        HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_buffer, sizeof(rx_buffer));
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        dma_state = 0;
    }
}
