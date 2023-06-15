#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32g431xx.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_i2c.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void Error_Handler(void);



