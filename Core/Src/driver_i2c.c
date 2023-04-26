/*
 * driver_i2c.c
 *
 *  Created on: March 21, 2023
 *      Author: Jasdip Sekhon
 */

#include "driver_i2c.h"

typedef struct {
  I2C_TypeDef *const registers; // STM32 memory mapped registers for the I2C bus

  uint8_t slave_address;
  uint8_t starting_slave_memory_address;

  uint8_t *read;        // Used for reading I2C slave device
  const uint8_t *write; // Used for writing data to the I2C slave device
  size_t number_of_bytes_to_transfer;
} i2c_s;

// I2C1 SDA = PB7, SCL = PA15
static void gpio_init() {
	const uint32_t i2c_mode = 0x2;
	const uint32_t pull_up = 0x1;
	const uint32_t clear = 0x3;
	const uint32_t AF4 = 0x4;
	const uint32_t open_drain = 0x1;

	GPIOB->MODER &= ~(clear << 14);
	GPIOB->MODER |= (i2c_mode << 14);      // Set PB7 Pin Function as I2C
	GPIOB->AFR[0] |= (AF4 << 28); 	       // Set alternate function to AF4 (SDA)
	GPIOB->OTYPER |= (open_drain << 7);    // Set output type to open-drain
	GPIOB->PUPDR &= ~(clear << 14);
	GPIOB->PUPDR |= (pull_up << 14);       // Enable pull-up resistor for PB7

	GPIOA->MODER &= ~(clear << 30);
	GPIOA->MODER |= (i2c_mode << 30);      // Set PA15 Pin Function as I2C
	GPIOA->AFR[1] |= (AF4 << 28);          // Set alternate function to AF4 (SCL)
	GPIOB->OTYPER |= (open_drain << 15);   // Set output type to open-drain
	GPIOA->PUPDR &= ~(clear << 30);
	GPIOA->PUPDR |= (pull_up << 30);       // Enable pull-up resistor for PA15
}

static void enable_GPIO_and_I2C_clock() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN; // enable peripheral clock for GPIO A and B
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // enable peripheral clock for I2C1
}

void i2c__initialize() {
	enable_GPIO_and_I2C_clock();
	gpio_init();

	I2C1->CR1 &= ~I2C_CR1_PE;
	I2C1->TIMINGR = 0x00B01A4B;          // TODO: need to change timer value
	I2C1->CR1 &= ~I2C_CR1_ANFOFF;        // Enable analog filter
	I2C1->CR1 &= ~I2C_CR1_DNF;
	I2C1->CR1 |= 0x3 << I2C_CR1_DNF_Pos; // Set digital noise filter
	I2C1->OAR1 = 0;
	I2C1->OAR2 = 0;
	I2C1->CR2 = 0;
	I2C1->CR1 &= I2C_CR1_PE;
}

uint8_t i2c__read_slave_data(uint8_t slave_address, uint8_t register_address) {
	uint8_t data = 0;
	I2C1->CR2 = (slave_address << 1) | I2C_CR2_START | I2C_CR2_AUTOEND; // Send start condition and slave address with write bit
	while (!(I2C1->ISR & I2C_ISR_TXIS)); // Wait for end of address transmission
	I2C1->TXDR = register_address;       // Send register address
	while (!(I2C1->ISR & I2C_ISR_TC));   // Wait for end of data transmission
	I2C1->CR2 = (slave_address << 1) | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_AUTOEND; // Send start and request read
	while (!(I2C1->ISR & I2C_ISR_RXNE)); // Wait for end of address transmission
	data = I2C1->RXDR;
	return data;
}

void i2c__write_slave_data(uint8_t slave_address, uint8_t register_address, uint8_t data) {
	while((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY); // Wait until I2C bus is ready
        I2C1->CR2 = (slave_address << 1) | I2C_CR2_AUTOEND | ((0x2 << I2C_CR2_NBYTES_Pos)); // Send start condition and slave address
    	I2C1->CR2 |= I2C_CR2_START;
    	while((I2C1->ISR & I2C_ISR_TXIS) == 0);  // Wait for TXIS flag to be set (transmit register address)
    	I2C1->TXDR = reg_address;                // Write the register address to the DR register
    	while((I2C1->ISR & I2C_ISR_TXIS) == 0);  // Wait for TXIS flag to be set (transmit data)
    	I2C1->TXDR = data; 						 // Write the data to the DR register
    	while((I2C1->ISR & I2C_ISR_STOPF) == 0); // Wait for STOP flag to be set
    	I2C1->ICR |= I2C_ICR_STOPCF;             // Clear the STOP flag by writing 0 to it
}
