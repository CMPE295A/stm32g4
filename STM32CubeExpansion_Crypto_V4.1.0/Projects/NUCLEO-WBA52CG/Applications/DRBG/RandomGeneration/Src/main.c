/**
  ******************************************************************************
  * @file    DRBG/RandomGeneration/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the
  *          STM32 Cryptographic Library
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "cmox_crypto.h"

/* Global variables ----------------------------------------------------------*/
/* DRBG context handle */
cmox_ctr_drbg_handle_t Drbg_Ctx;

__IO TestStatus glob_status = FAILED;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Extract from CTR_DRBG.rsp
  * [AES-256 use df]
[PredictionResistance = False]
[EntropyInputLen = 256]
[NonceLen = 128]
[PersonalizationStringLen = 256]
[AdditionalInputLen = 0]
[ReturnedBitsLen = 512]

COUNT = 0
EntropyInput = 4cfb218673346d9d50c922e49b0dfcd090adf04f5c3ba47327dfcd6fa63a785c
Nonce = 016962a7fd2787a24bf6be47ef3783f1
PersonalizationString = 88eeb8e0e83bf3294bdacd6099ebe4bf55ecd9113f71e5ebcb4575f3d6a68a6b
EntropyInputReseed = b7ec46072363834a1b0133f2c23891db4f11a68651f23e3a8b1fdc03b192c7e7
AdditionalInputReseed =
AdditionalInput =
AdditionalInput =
ReturnedBits = a55180a190bef3adaf28f6b795e9f1f3d6dfa1b27dd0467b0c75f5fa931e971475b27
cae03a29654e2f40966ea33643040d1400fe677873af8097c1fe9f00298

  */
const uint8_t Entropy[] =
{
  0x4c, 0xfb, 0x21, 0x86, 0x73, 0x34, 0x6d, 0x9d, 0x50, 0xc9, 0x22, 0xe4, 0x9b, 0x0d, 0xfc, 0xd0,
  0x90, 0xad, 0xf0, 0x4f, 0x5c, 0x3b, 0xa4, 0x73, 0x27, 0xdf, 0xcd, 0x6f, 0xa6, 0x3a, 0x78, 0x5c
};
const uint8_t Nonce[] =
{
  0x01, 0x69, 0x62, 0xa7, 0xfd, 0x27, 0x87, 0xa2, 0x4b, 0xf6, 0xbe, 0x47, 0xef, 0x37, 0x83, 0xf1
};
const uint8_t Personalization[] =
{
  0x88, 0xee, 0xb8, 0xe0, 0xe8, 0x3b, 0xf3, 0x29, 0x4b, 0xda, 0xcd, 0x60, 0x99, 0xeb, 0xe4, 0xbf,
  0x55, 0xec, 0xd9, 0x11, 0x3f, 0x71, 0xe5, 0xeb, 0xcb, 0x45, 0x75, 0xf3, 0xd6, 0xa6, 0x8a, 0x6b
};
const uint8_t EntropyInputReseed[] =
{
  0xb7, 0xec, 0x46, 0x07, 0x23, 0x63, 0x83, 0x4a, 0x1b, 0x01, 0x33, 0xf2, 0xc2, 0x38, 0x91, 0xdb,
  0x4f, 0x11, 0xa6, 0x86, 0x51, 0xf2, 0x3e, 0x3a, 0x8b, 0x1f, 0xdc, 0x03, 0xb1, 0x92, 0xc7, 0xe7
};
const uint8_t Known_Random[] =
{
  0xa5, 0x51, 0x80, 0xa1, 0x90, 0xbe, 0xf3, 0xad, 0xaf, 0x28, 0xf6, 0xb7, 0x95, 0xe9, 0xf1, 0xf3,
  0xd6, 0xdf, 0xa1, 0xb2, 0x7d, 0xd0, 0x46, 0x7b, 0x0c, 0x75, 0xf5, 0xfa, 0x93, 0x1e, 0x97, 0x14,
  0x75, 0xb2, 0x7c, 0xae, 0x03, 0xa2, 0x96, 0x54, 0xe2, 0xf4, 0x09, 0x66, 0xea, 0x33, 0x64, 0x30,
  0x40, 0xd1, 0x40, 0x0f, 0xe6, 0x77, 0x87, 0x3a, 0xf8, 0x09, 0x7c, 0x1f, 0xe9, 0xf0, 0x02, 0x98
};

/* Computed data buffer */
uint8_t Computed_Random[sizeof(Known_Random)];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  cmox_drbg_retval_t retval;
  /* General DRBG context */
  cmox_drbg_handle_t *drgb_ctx;


  /* STM32WBAxx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock */
  SystemClock_Config();


  /* Enable instruction cache (default 2-ways set associative cache) */
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure LD3 */
  BSP_LED_Init(LD3);

  /* Initialize cryptographic library */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   * KNOWN RANDOM PRODUCTION
   * --------------------------------------------------------------------------
   */

  /* Construct a drbg context that is configured to perform ctrDRBG with AES256 operations */
  drgb_ctx = cmox_ctr_drbg_construct(&Drbg_Ctx, CMOX_CTR_DRBG_AES256);
  if (drgb_ctx == NULL)
  {
    Error_Handler();
  }

  /* Initialize the DRBG context with entropy, nonce and personalization string parameters */
  retval = cmox_drbg_init(drgb_ctx,                                     /* DRBG context */
                          Entropy, sizeof(Entropy),                     /* Entropy data */
                          Personalization, sizeof(Personalization),     /* Personalization string */
                          Nonce, sizeof(Nonce));                        /* Nonce data */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    Error_Handler();
  }

  /* Reseed the DRBG with reseed parameters */
  retval = cmox_drbg_reseed(drgb_ctx,                                           /* DRBG context */
                            EntropyInputReseed, sizeof(EntropyInputReseed),     /* Entropy reseed data */
                            NULL, 0);                                           /* No additional reseed data */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    Error_Handler();
  }

  /* Generate 1st random data */
  retval = cmox_drbg_generate(drgb_ctx,                                    /* DRBG context */
                              NULL, 0,                                     /* No additional data */
                              Computed_Random, sizeof(Computed_Random));   /* Data buffer to receive generated random */

  /* Verify API returned value */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    Error_Handler();
  }

  /* Generate 2nd random data */
  retval = cmox_drbg_generate(drgb_ctx,                                    /* DRBG context */
                              NULL, 0,                                     /* No additional data */
                              Computed_Random, sizeof(Computed_Random));   /* Data buffer to receive generated random */

  /* Verify API returned value */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Random, Known_Random, sizeof(Known_Random)) != 0)
  {
    Error_Handler();
  }

  /* Cleanup the context */
  retval = cmox_drbg_cleanup(drgb_ctx);
  if (retval != CMOX_DRBG_SUCCESS)
  {
    Error_Handler();
  }

  /* No more need of cryptographic services, finalize cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  /* Turn on LD3 in an infinite loop in case of AES CBC operations are successful */
  BSP_LED_On(LD3);
  glob_status = PASSED;
  while (1)
  {}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            AHB5 Prescaler                 = 4
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB7 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 2
  *            PLL_N                          = 25
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 3
  *            Voltage range                  = 1
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  /** At reset, the regulator is the LDO, in range 2
     Need to move to range 1 to reach 100 MHz
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization error */
    Error_Handler();
  }

  /** Activate PLL with HSI as source
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLFractional = 0U;
  RCC_OscInitStruct.PLL1.PLLM = 2U;
  RCC_OscInitStruct.PLL1.PLLN = 25U;   /* VCO = HSI/M * N = 16 / 2 * 25 = 200 MHz */
  RCC_OscInitStruct.PLL1.PLLR = 2U;    /* PLLSYS = 200 MHz / 2 = 100 MHz */
  RCC_OscInitStruct.PLL1.PLLP = 2U;
  RCC_OscInitStruct.PLL1.PLLQ = 2U;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization error */
    Error_Handler();
  }

  /** Select PLL as system clock source and configure the HCLK, PCLK1, PCLK2 and PCLK7
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 \
                               | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK7 | RCC_CLOCKTYPE_HCLK5);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB7CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHB5_PLL1_CLKDivider = RCC_SYSCLK_PLL1_DIV4;
  RCC_ClkInitStruct.AHB5_HSEHSI_CLKDivider = RCC_SYSCLK_HSEHSI_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization error */
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  /* Toggle LD3 @2Hz to notify error condition */
  while (1)
  {
    BSP_LED_Toggle(LD3);
    HAL_Delay(250);
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
