/**
  ******************************************************************************
  * @file    entropy.c
  * @author  MCD Application Team
  * @brief   This file contains functions for Entropy gathering utility
  * @note    This solution should be used only if the target device doesn’t provide
  *          a TRNG peripheral, and its usage is discouraged for critical security
  *          or cryptographic purposes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "entropy.h"

/* Global variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htimx;
static __IO uint32_t EntropyIndex;
static __IO uint32_t EntropyMax;
static __IO uint8_t *pEntropyBuffer;
/* Private function prototypes -----------------------------------------------*/
static void RestartLSI(void);
static void ConfigureMeasureTimer(void);
static void StartMeasureTimer(void);
static void Error_Handler(void);
void TIM_IRQ_HANDLER(void);
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Entropy gathering utility
  * @param  nbBits    Number of entropy bits to gather
  * @param  pEntropy  Buffer to receive entropy data (must be enough large to contain nbBits bits)
  * @retval None
  */
void Entropy_Gather(uint32_t nbBits, __IO uint8_t *pEntropy)
{
  EntropyIndex = 0;
  EntropyMax = nbBits;
  pEntropyBuffer = pEntropy;

  /* Reset entropy buffer contents */
  for (uint32_t i = 0U; i < (((nbBits & 0x7UL) == 0UL) ? (nbBits / 8) : (nbBits / 8 + 1)); i++)
  {
    pEntropy[i] = 0U;
  }

  /* Configure timer to measure LSI cycle length */
  ConfigureMeasureTimer();

  /* Restart LSI oscillator */
  RestartLSI();

  /* Launch measurement timer */
  StartMeasureTimer();

  /* Wait for all entropy bit to be gathered */
  while (EntropyIndex < EntropyMax)
  {}
}

/**
  * @brief  LSI restart utility
  * @param  None
  * @retval None
  */
static void RestartLSI(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Configure timer to measure LSI cycle length
  * @param  None
  * @retval None
  */
static void ConfigureMeasureTimer(void)
{
  TIM_IC_InitTypeDef sConfigIC = {0};

  htimx.Instance = TIM10;
  htimx.Init.Prescaler = 0;
  htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx.Init.Period = 0xFFFFU; /* Set Max Period */
  htimx.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htimx.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htimx) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htimx) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htimx, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIMEx_RemapConfig(&htimx, TIM_TIM10_LSI | TIM_TIM10_TI1RMP) != HAL_OK)
  {
    Error_Handler();
  }
  /* TIM interrupt Init */
  HAL_NVIC_SetPriority(TIM10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM10_IRQn);
}

/**
  * @brief  Start timer to measure LSI cycle length
  * @param  None
  * @retval None
  */
static void StartMeasureTimer(void)
{
  if (HAL_TIM_IC_Start_IT(&htimx, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Input Capture callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  uint32_t meas = HAL_TIM_ReadCapturedValue(&htimx, TIM_CHANNEL_1);
  HAL_TIM_IC_Stop_IT(&htimx, TIM_CHANNEL_1);

  pEntropyBuffer[EntropyIndex / 8] |= (uint8_t)((meas & 0x01UL) << (7 - (EntropyIndex & 0x07UL)));
  EntropyIndex++;

  if (EntropyIndex < EntropyMax)
  {
    RestartLSI();
    StartMeasureTimer();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  /* Toggle LED2 @2Hz to notify error condition */
  while (1)
  {
    BSP_LED_Toggle(LED2);
    HAL_Delay(250);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
