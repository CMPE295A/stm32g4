/**
  ******************************************************************************
  * @file    MAC/HMAC_SHA2_AuthenticateVerify/Src/main.c
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
/* HMAC context handle */
cmox_hmac_handle_t Hmac_Ctx;

__IO TestStatus glob_status = FAILED;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define CHUNK_SIZE  48u   /* Chunk size (in bytes) when data to mac are processed by chunk */
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Extract from HMAC.rsp
  *

Klen = 40
Tlen = 32
Key = cfd4a44910c9e567507abb6cede4fe601a7a2765c9755aa2cf6ba4814223811a26a8a1ef499cebd9
Msg = 3fb301cb4092f9623aa5ffd690d22d65d56e5a1c330b9c4a0d910c34e391c90a76d5401a2d3caa44b8c5d5aef3e928b
90d2ee233e9f9a2cec4a32cd019d06a0dc1fcb1125f5746a4fbd32169ed7bf0e4fd065fa7c8ac97c366380484495f5c5b6850
dd1c9d8cd6694cf8686e46308ed0ed1f5bdf98cd831339771db63de5a7de
Mac = 20153bf8ea2953c48251ebcc4161f8b6e28499e5c76c24014cff4a9e2f62d25c

  */
const uint8_t Key[] =
{
  0xcf, 0xd4, 0xa4, 0x49, 0x10, 0xc9, 0xe5, 0x67, 0x50, 0x7a, 0xbb, 0x6c, 0xed, 0xe4, 0xfe, 0x60,
  0x1a, 0x7a, 0x27, 0x65, 0xc9, 0x75, 0x5a, 0xa2, 0xcf, 0x6b, 0xa4, 0x81, 0x42, 0x23, 0x81, 0x1a,
  0x26, 0xa8, 0xa1, 0xef, 0x49, 0x9c, 0xeb, 0xd9
};
const uint8_t Message[] =
{
  0x3f, 0xb3, 0x01, 0xcb, 0x40, 0x92, 0xf9, 0x62, 0x3a, 0xa5, 0xff, 0xd6, 0x90, 0xd2, 0x2d, 0x65,
  0xd5, 0x6e, 0x5a, 0x1c, 0x33, 0x0b, 0x9c, 0x4a, 0x0d, 0x91, 0x0c, 0x34, 0xe3, 0x91, 0xc9, 0x0a,
  0x76, 0xd5, 0x40, 0x1a, 0x2d, 0x3c, 0xaa, 0x44, 0xb8, 0xc5, 0xd5, 0xae, 0xf3, 0xe9, 0x28, 0xb9,
  0x0d, 0x2e, 0xe2, 0x33, 0xe9, 0xf9, 0xa2, 0xce, 0xc4, 0xa3, 0x2c, 0xd0, 0x19, 0xd0, 0x6a, 0x0d,
  0xc1, 0xfc, 0xb1, 0x12, 0x5f, 0x57, 0x46, 0xa4, 0xfb, 0xd3, 0x21, 0x69, 0xed, 0x7b, 0xf0, 0xe4,
  0xfd, 0x06, 0x5f, 0xa7, 0xc8, 0xac, 0x97, 0xc3, 0x66, 0x38, 0x04, 0x84, 0x49, 0x5f, 0x5c, 0x5b,
  0x68, 0x50, 0xdd, 0x1c, 0x9d, 0x8c, 0xd6, 0x69, 0x4c, 0xf8, 0x68, 0x6e, 0x46, 0x30, 0x8e, 0xd0,
  0xed, 0x1f, 0x5b, 0xdf, 0x98, 0xcd, 0x83, 0x13, 0x39, 0x77, 0x1d, 0xb6, 0x3d, 0xe5, 0xa7, 0xde
};
const uint8_t Expected_Tag[] =
{
  0x20, 0x15, 0x3b, 0xf8, 0xea, 0x29, 0x53, 0xc4, 0x82, 0x51, 0xeb, 0xcc, 0x41, 0x61, 0xf8, 0xb6,
  0xe2, 0x84, 0x99, 0xe5, 0xc7, 0x6c, 0x24, 0x01, 0x4c, 0xff, 0x4a, 0x9e, 0x2f, 0x62, 0xd2, 0x5c
};

/* Computed data buffer */
uint8_t Computed_Tag[sizeof(Expected_Tag)];

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
  cmox_mac_retval_t retval;
  size_t computed_size;
  /* General mac context */
  cmox_mac_handle_t *mac_ctx;
  /* Index for piecemeal processing */
  uint32_t index;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_MAC_AUTH_FAIL;

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
   * SINGLE CALL USAGE
   * --------------------------------------------------------------------------
   */

  /* Compute directly the authentication tag passing all the needed parameters */
  retval = cmox_mac_compute(CMOX_HMAC_SHA256_ALGO,     /* Use HMAC SHA256 algorithm */
                            Message, sizeof(Message),  /* Message to authenticate */
                            Key, sizeof(Key),          /* HMAC Key to use */
                            NULL, 0,                   /* Custom data */
                            Computed_Tag,              /* Data buffer to receive generated authnetication tag */
                            sizeof(Expected_Tag),      /* Expected authentication tag size */
                            &computed_size);           /* Generated tag size */

  /* Verify API returned value */
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Tag))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Tag, Computed_Tag, computed_size) != 0)
  {
    Error_Handler();
  }

  /* Verify directly the message passing all the needed parameters */
  retval = cmox_mac_verify(CMOX_HMAC_SHA256_ALGO,     /* Use HMAC SHA256 algorithm */
                           Message, sizeof(Message),  /* Message to authenticate */
                           Key, sizeof(Key),          /* HMAC Key to use */
                           NULL, 0,                   /* Custom data */
                           Expected_Tag,              /* Authentication tag */
                           sizeof(Expected_Tag));     /* tag size */

  /* Verify API returned value */
  if (retval != CMOX_MAC_AUTH_SUCCESS)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   * MULTIPLE CALLS USAGE
   * --------------------------------------------------------------------------
   */

  /* Construct a MAC context that is configured to perform HMAC SHA256 authentication operations */
  mac_ctx = cmox_hmac_construct(&Hmac_Ctx, CMOX_HMAC_SHA256);
  if (mac_ctx == NULL)
  {
    Error_Handler();
  }

  /* Initialize the MAC context */
  retval = cmox_mac_init(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Set the desired size for the authentication tag to compute: note that in the case
     where the size of the tag is the default for the algorithm, it is
     possible to skip this call. */
  retval = cmox_mac_setTagLen(mac_ctx, sizeof(Expected_Tag));
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Setup of the authentication key into the context */
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* HMAC Key to use */
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Append the message to be authenticated by chunks of CHUNK_SIZE Bytes */
  for (index = 0; index < (sizeof(Message) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], CHUNK_SIZE); /* Chunk of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      Error_Handler();
    }
  }
  /* Append the last part of the message if needed */
  if (index < sizeof(Message))
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], sizeof(Message) - index); /* Last part of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      Error_Handler();
    }
  }

  /* Generate the authentication tag */
  retval = cmox_mac_generateTag(mac_ctx, Computed_Tag, &computed_size);

  /* Verify API returned value */
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Tag))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Tag, Computed_Tag, computed_size) != 0)
  {
    Error_Handler();
  }

  /* Cleanup the context */
  retval = cmox_mac_cleanup(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Construct a MAC context that is configured to perform HMAC SHA256 authentication operations */
  mac_ctx = cmox_hmac_construct(&Hmac_Ctx, CMOX_HMAC_SHA256);
  if (mac_ctx == NULL)
  {
    Error_Handler();
  }

  /* Initialize the MAC context */
  retval = cmox_mac_init(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Set the desired size for the authentication tag to compute: note that in the case
     where the size of the tag is the default for the algorithm, it is
     possible to skip this call. */
  retval = cmox_mac_setTagLen(mac_ctx, sizeof(Expected_Tag));
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Setup of the authentication key into the context */
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* HMAC Key to use */
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* Append the message to be authenticated by chunks of CHUNK_SIZE Bytes */
  for (index = 0; index < (sizeof(Message) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], CHUNK_SIZE); /* Chunk of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      Error_Handler();
    }
  }
  /* Append the last part of the message if needed */
  if (index < sizeof(Message))
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], sizeof(Message) - index); /* Last part of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      Error_Handler();
    }
  }

  /* Verify the authentication tag */
  retval = cmox_mac_verifyTag(mac_ctx,
                              Expected_Tag,   /* Authentication tag used for verification */
                              &fault_check);  /* Fault check variable:
                                              to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_MAC_AUTH_SUCCESS)
  {
    Error_Handler();
  }
  /* Verify Fault check variable value */
  if (fault_check != CMOX_MAC_AUTH_SUCCESS)
  {
    Error_Handler();
  }

  /* Cleanup the context */
  retval = cmox_mac_cleanup(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    Error_Handler();
  }

  /* No more need of cryptographic services, finalize cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  /* Turn on LD3 in an infinite loop in case of HMAC SHA256 operations are successful */
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
