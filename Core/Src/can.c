/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    can.c
 * @brief   This file provides code for the configuration
 *          of the CAN instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  CAN_FilterTypeDef CAN_FilterConfig = {0};//定义过滤器
  /*---------------过滤器1-----------------*/
  CAN_FilterConfig.FilterActivation = ENABLE;               // 激活过滤器
  CAN_FilterConfig.SlaveStartFilterBank = 14;               // CAN1 CAN2的过滤器分割线，0~13分给CAN1，14-27分给CAN2
  CAN_FilterConfig.FilterBank = 0;                          // 使用第0个过滤器组
  CAN_FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;     // 位宽
  CAN_FilterConfig.FilterMode = CAN_FILTERMODE_IDLIST; // 模式（列表/掩码）
  CAN_FilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 用哪个FIFO的信箱
  //FR0 FR1（32位）标准帧11位[15-5] 11位ID [4-0] 其他
  //扩展帧29位id [31-3] 29位ID [2-0] 其他:[2]:IDE(0:标准帧 1:扩展帧)
  //((0x01010203)<<3 | 4)
  //((0x01010203)<<3 | 4) & 0xFFFF//低16位
  //((0x01010203)<<3 | 4)>>16//高16位
  //0x000-0x7FF 11位标准帧
  //0x00000000-0x1FFFFFFF 29位扩展帧
  CAN_FilterConfig.FilterIdHigh = (((0x01020101)<<3 | 4)>>16);            // 基准高位 FR0 高16位
  CAN_FilterConfig.FilterMaskIdHigh =(((0x01020201)<<3 | 4)>>16);     // 掩码高位 FR0 低16位
  CAN_FilterConfig.FilterIdLow = (((0x01020101)<<3 | 4) & 0xFFFF);              // 基准低位 FR1 高16位
  CAN_FilterConfig.FilterMaskIdLow = (((0x01020201)<<3 | 4) & 0xFFFF);  // 掩码低位 FR1 低16位
  

//  CAN_FilterConfig.FilterIdHigh = (0x201<<5);            // 基准高位 FR0 高16位
//  CAN_FilterConfig.FilterMaskIdHigh =(0x202<<5);     // 掩码高位 FR0 低16位
//  CAN_FilterConfig.FilterIdLow = (0x203<<5);              // 基准低位 FR1 高16位
//  CAN_FilterConfig.FilterMaskIdLow = (0x204<<5);

  if (HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterConfig) != HAL_OK)
  { // 应用硬件中
    Error_Handler();
  }

    /*---------------过滤器2-----------------*/
  CAN_FilterConfig.FilterActivation = ENABLE;               // 激活过滤器
  CAN_FilterConfig.SlaveStartFilterBank = 14;               // CAN1 CAN2的过滤器分割线，0~13分给CAN1，14-27分给CAN2
  CAN_FilterConfig.FilterBank = 1;                        // 使用第0个筛选器组
  CAN_FilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;     // 位宽
  CAN_FilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;      // 模式（列表/掩码）
  CAN_FilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 用哪个FIFO的信箱
  CAN_FilterConfig.FilterIdHigh = (0x201 << 5);             // 基准高位
  CAN_FilterConfig.FilterMaskIdHigh = (0X202 << 5);         // 掩码高位
  CAN_FilterConfig.FilterIdLow = (0x203 << 5);              // 基准低位
  CAN_FilterConfig.FilterMaskIdLow = (0x204 << 5);          // 掩码低位


  if (HAL_CAN_Start(&hcan1) != HAL_OK)
    Error_Handler();
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

