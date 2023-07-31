/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "driver_uart.h"
#include "driver_timer.h"
#include "cmox_crypto.h"
#include "interface_at_command.h"
#include "interface_mqtt.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void process_cli(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

void wait_ms(int ms_wait_threshold)
{
	  uint16_t modem_reset_count = 0;
	  while(modem_reset_count < ms_wait_threshold)
	  {
		  if(timer__ms_elapsed(DRIVER_TIMER2))
		  {
			  modem_reset_count++;
		  }
	  }
}

int main(void)
{
  /* USER CODE BEGIN 1 */

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
  timer__initialize(DRIVER_TIMER2);

  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  wait_ms(1000);
  HAL_GPIO_WritePin(WIFI_MODEM_RESET_PORT, WIFI_MODEM_RESET_PIN, GPIO_PIN_SET);
  wait_ms(1000);
  at_interface__initialize();

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int count = 0;
  static const char TEST_STRING[] = "\"{\"GPS\":[37.3387,-121.8853],\"Battery\":45%,\"Temperature\": 21.1C}\"";
  while (1)
  {
	  bool ms_elapsed = false;
	  if(timer__ms_elapsed(DRIVER_TIMER2))
	  {
		  ms_elapsed = true;
		  mqtt__process();
		  if(++count == 1000)
		  {
			  count = 0;
//			  at_interface__publish_test();
			  mqtt__publish(TEST_STRING, strlen(TEST_STRING));
		  }
	  }
	  at_interface__process(ms_elapsed);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WIFI_MODEM_RESET_PORT, WIFI_MODEM_RESET_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : A2 Wiznet360 reset */
  GPIO_InitStruct.Pin = WIFI_MODEM_RESET_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WIFI_MODEM_RESET_PORT, &GPIO_InitStruct);

  //  // USART1_TX (A9)
  GPIOA->MODER &= ~(3 << 18);
  GPIOA->MODER |= (2 << 18); // alternate function
  GPIOA->OTYPER &= ~(1 << 9); // push-pull
  GPIOA->PUPDR &= ~(3 << 18); // no pull
  GPIOA->OSPEEDR &= ~(3 << 18); //
  GPIOA->OSPEEDR |= (3 << 18); // very high frequency
  GPIOA->AFR[1] |= (7 << 4);
  //
  //  // USART1_RX (A10)
  GPIOA->MODER &= ~(3 << 20);
  GPIOA->MODER |= (2 << 20); // alternate function
  GPIOA->OTYPER &= ~(1 << 10); // push-pull
  GPIOA->PUPDR &= ~(3 << 20); // no pull
  GPIOA->OSPEEDR &= ~(3 << 20);
  GPIOA->OSPEEDR |= (3 << 20); // very high frequency
  GPIOA->AFR[1] |= (7 << 8);

//  //  // USART2_TX (A2)
//  GPIOA->MODER &= ~(3 << 4);
//  GPIOA->MODER |= (2 << 4); // alternate function
//  GPIOA->OTYPER &= ~(1 << 2); // push-pull
//  GPIOA->PUPDR &= ~(3 << 4); // no pull
//  GPIOA->OSPEEDR &= ~(3 << 4); //
//  GPIOA->OSPEEDR |= (3 << 4); // very high frequency
//  GPIOA->AFR[1] |= (7 << 8);
//  //
//  //  // USART2_RX (A3)
//  GPIOA->MODER &= ~(3 << 6);
//  GPIOA->MODER |= (2 << 6); // alternate function
//  GPIOA->OTYPER &= ~(1 << 3); // push-pull
//  GPIOA->PUPDR &= ~(3 << 6); // no pull
//  GPIOA->OSPEEDR &= ~(3 << 6);
//  GPIOA->OSPEEDR |= (3 << 6); // very high frequency
//  GPIOA->AFR[1] |= (7 << 12);
}

static void process_cli(void)
{
	static uint16_t index = 0;
	static char buff[1024] = {0};
	uint8_t c = uart__get_char(DRIVER_UART1);
	if(c != NO_CHAR_AVAILABLE)
	{
	  // echo
	  uart__put(DRIVER_UART1, &c, 1);
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
