/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TIM_IRQ_Handler.h"
#include "switch_sm.h"
#include "UART_IRQ_Handler.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define CH_COUNT 1
typedef struct Frame
{
    float fdata[CH_COUNT];
    uint8_t tail[4];
}Frame;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CAN_RX_DEBUG_ENABLE 0


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t switch_state =0 ;
Frame frame={0,{0x00, 0x00, 0x80, 0x7F}};
double t = 0;
uint8_t volatile dma_state = 0;
uint32_t last_time = 0;

CAN_TxHeaderTypeDef txHeader_Buzzer;

uint32_t txMailbox;
uint8_t Buzzer_Data[8] = {0};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if CAN_RX_DEBUG_ENABLE
typedef struct
{
  uint32_t tick;
  HAL_CAN_StateTypeDef state;
  uint32_t error_code;
  uint32_t ier;
  uint32_t rf0r;
  uint32_t esr;
  uint32_t msr;
  uint32_t btr;
  uint32_t fifo0_level;
  uint32_t rx0_irq_enabled;
  uint32_t rx0_irq_pending;
} CAN_RxDebugTypeDef;

static volatile CAN_RxDebugTypeDef can_rx_debug;

static void CAN_RxDebugCheck(void)
{
  static uint32_t last_check_tick = 0;
  uint32_t current_tick = HAL_GetTick();

  if ((current_tick - last_check_tick) < 50U)
  {
    return;
  }

  last_check_tick = current_tick;
  can_rx_debug.tick = current_tick;
  can_rx_debug.state = HAL_CAN_GetState(&hcan1);
  can_rx_debug.error_code = HAL_CAN_GetError(&hcan1);
  can_rx_debug.ier = CAN1->IER;
  can_rx_debug.rf0r = CAN1->RF0R;
  can_rx_debug.esr = CAN1->ESR;
  can_rx_debug.msr = CAN1->MSR;
  can_rx_debug.btr = CAN1->BTR;
  can_rx_debug.fifo0_level = HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0);
  can_rx_debug.rx0_irq_enabled = NVIC_GetEnableIRQ(CAN1_RX0_IRQn);
  can_rx_debug.rx0_irq_pending = NVIC_GetPendingIRQ(CAN1_RX0_IRQn);
}
#endif

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /*蜂鸣器响完后发送的数据*/
  txHeader_Buzzer.IDE = CAN_ID_EXT;
  txHeader_Buzzer.ExtId = 0x02010101;       // 29位扩展ID
  txHeader_Buzzer.RTR = CAN_RTR_DATA;
  txHeader_Buzzer.DLC = 2;
  txHeader_Buzzer.TransmitGlobalTime = DISABLE;
  Buzzer_Data[0]='O';
  Buzzer_Data[1]='K';
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  UART_Start_Receive();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if CAN_RX_DEBUG_ENABLE
    CAN_RxDebugCheck();
#endif

		
    if(Beep_Trigger!=0)
    {
      Beep_Alarm(Beep_Trigger);
      HAL_CAN_AddTxMessage(&hcan1,&txHeader_Buzzer,Buzzer_Data,&txMailbox);
      Beep_Trigger = 0;
    }//蜂鸣器按上位机发出的满足条件的0x01的数目发声
    if(switch_state==FLOW_STATE)
    {
      state_run(FLOW_STATE);

    }
    else if (switch_state==IDLE_STATE)
    {
      state_run(IDLE_STATE);
      /* code */
    }
		if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)!=0)
          HAL_CAN_AddTxMessage(&hcan1,&txHeader_Buzzer,Buzzer_Data,&txMailbox);

    


    /*if(tx_flag == 1)
    {
      HAL_UART_Transmit(&huart1,tx_buffer,BUFFER_LEN,200);
      tx_flag =0;
    }*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 13;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  GPIO_EXTI_Callback(GPIO_Pin,&switch_state);

}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  TIM_PeriodElapsedCallback(htim);
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
