#include "CAN_IRQ_Handler.h"
#include "main.h"

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
static uint32_t txMailbox;
uint8_t Flow_Data[8] = {0};

CAN_TxHeaderTypeDef txHeader_Flow;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // 确保是CAN1触发的中断
    if (hcan->Instance == CAN1)
    {
        // 从FIFO 0 把数据捞出来，存到RxData 数组里
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
            if (RxHeader.IDE == CAN_ID_EXT &&
                RxHeader.ExtId == 0x01020101)
            {
                Beep_Trigger = RxData[0];
            }
            else if (RxHeader.IDE == CAN_ID_EXT &&
                     RxHeader.ExtId == 0x01020201)
            {
                txHeader_Flow.IDE = CAN_ID_EXT;
                txHeader_Flow.ExtId = 0x02010201; // 29位扩展ID
                txHeader_Flow.RTR = CAN_RTR_DATA;
                txHeader_Flow.DLC = 3;
                txHeader_Flow.TransmitGlobalTime = DISABLE;
                Flow_Data[0] = 'O';
                Flow_Data[1] = 'K';
                if (RxHeader.RTR == CAN_RTR_DATA && RxHeader.DLC >= 1)
                {
                    if (RxData[0] == 1)
                    {
                        switch_state = FLOW_STATE;
                        Flow_Data[2] = 1;
                        HAL_CAN_AddTxMessage(&hcan1, &txHeader_Flow, Flow_Data, &txMailbox);
                    }
                    if (RxData[0] == 0)
                    {
                        switch_state = IDLE_STATE;
                        Flow_Data[2] = 0;
                        HAL_CAN_AddTxMessage(&hcan1, &txHeader_Flow, Flow_Data, &txMailbox);
                    }
                }
            }
            if(RxHeader.StdId==0x200)
            {
                 Beep_Trigger = 1;
            }
        }
    }
    else if (hcan->Instance == CAN2)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
        }
    }
}