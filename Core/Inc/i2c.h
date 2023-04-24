/*
 * i2c.h
 *
 *  Created on: March 21, 2023
 *      Author: Jasdip Sekhon
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32g431xx.h"

void i2c__initialize(uint32_t desired_i2c_bus_speed_in_hz, uint32_t peripheral_clock_hz);

/**
 * @returns true if the I2C device at the given address responds back with an ACK
 */
bool i2c__detect(uint8_t slave_address);

/**
 * Reads multiple registers of the slave_address
 */
uint8_t i2c__read_slave_data(uint8_t slave_address, uint8_t register_address);


/**
 * Writes multiple registers of the slave_address
 */
uint8_t i2c__write_slave_data(uint8_t slave_address, uint8_t register_address, uint8_t data);
