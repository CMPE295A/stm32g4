/*
 * i2c.h
 *
 *  Created on: March 21, 2023
 *      Author: Jasdip Sekhon
 */

#include "i2c.h"

typedef enum {
    I2C__STATE_IDLE,
    I2C__STATE_START,
    I2C__STATE_SEND_SLAVE_ADDR,
	I2C__STATE_SEND_REGISTER_ADDR,
	I2C__STATE_REPEATED_START_CONDITION,
	I2C__STATE_RECEIVE_DATA,
    I2C__STATE_STOP,
	I2C__STATE_ERROR,
} i2c_state_e;


volatile i2c_state_e i2c_state = I2C__STATE_IDLE;
static uint8_t data;

void I2C1_EV_IRQHandler(uint8_t slave_address, uint8_t register_address, uint32_t number_of_bytes) {
	switch(i2c_state) {
	case I2C__STATE_IDLE:
		break;

	case I2C__STATE_START:
		while (!(I2C1->ISR & I2C_ISR_BUSY));
		I2C1->CR2 |= I2C_CR2_START; // Send start condition
		while (!(I2C1->ISR & I2C_ISR_BUSY));
		i2c_state = I2C__STATE_SEND_SLAVE_ADDR;
		break;

	case I2C__STATE_SEND_SLAVE_ADDR:
		I2C1->CR2 = (slave_address << 1) | I2C_CR2_RD_WRN; // Send slave address with read bit set
		while (!(I2C1->ISR & I2C_ISR_ADDR)); // Wait for slave AWK
		if (I2C1->ISR & I2C_ISR_NACKF) { // Slave did not AWK
			i2c_state = I2C__STATE_ERROR;
		    break;
		}
		i2c_state = I2C__STATE_RECEIVE_DATA;
		break;

	case I2C__STATE_RECEIVE_DATA:
		while (!(I2C1->ISR & I2C_ISR_RXNE)); // Wait for data to be received
		data = I2C1->RXDR; // read data
		number_of_bytes--;
		if (number_of_bytes == 0) { // No more bytes to read
			I2C1->CR2 |= I2C_CR2_STOP;
			i2c_state = I2C__STATE_STOP;
		}
		else if (number_of_bytes == 1) { // Send NACK if 1 last byte
			I2C1->CR2 |= I2C_CR2_NACK;
		}
		else {I2C1->CR2 &= ~I2C_CR2_NACK;} // More bytes to AWK
		i2c_state = I2C__STATE_REPEATED_START_CONDITION;
		break;

	case I2C__STATE_STOP:
		I2C1->CR2 |= I2C_CR2_STOP;
		while (I2C1->CR2 & I2C_CR2_STOP); // wait for STOP condition to be cleared internally
		while (!(I2C1->ISR & I2C_ISR_STOPF));
		I2C1->ICR |= I2C_ICR_ADDRCF | I2C_ICR_NACKCF | I2C_ICR_STOPCF | I2C_ICR_BERRCF // Clear pending flags in ICR register
				| I2C_ICR_ARLOCF | I2C_ICR_OVRCF| I2C_ICR_PECCF | I2C_ICR_TIMOUTCF | I2C_ICR_ALERTCF;
		i2c_state = I2C__STATE_IDLE;
		break;

	case I2C__STATE_REPEATED_START_CONDITION:
	    I2C1->CR2 |= I2C_CR2_START; // Send repeated start condition
	    while (!(I2C1->ISR & I2C_ISR_BUSY)); // Wait for start condition to be sent
	    i2c_state = I2C__STATE_SEND_SLAVE_ADDR;
	    break;

	case I2C__STATE_ERROR:
		i2c_state = I2C__STATE_IDLE;
		break;

	default:
		break;
	}
}

// I2C1 SDA = PB7, SCL = PA15
static void gpio_init(void) {
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

static void enable_GPIO_and_I2C_clock(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN; // enable peripheral clock for GPIO A and B
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // enable peripheral clock for I2C1
}

static void congifure_I2C_clock(void) {
	const uint32_t prescaler = 4;
	const uint32_t SCLDEL = 4;
	const uint32_t SDADEL = 2;
	const uint32_t SCLH = 67;
	const uint32_t SCLL = 134;

	// Configure I2C1_TIMINGR for 100KHz speed
	I2C1->TIMINGR = (prescaler << 28) |
					(SCLDEL << 20) |
					(SDADEL << 16) |
					(SCLH << 8) |
					(SCLL);
}

void i2c__initialize(void) {
	enable_GPIO_and_I2C_clock();
	gpio_init();

	I2C1->CR1 &= ~I2C_CR1_PE;
	congifure_I2C_clock();
	I2C1->CR1 &= ~I2C_CR1_ANFOFF;        // Enable analog filter
	I2C1->CR1 &= ~I2C_CR1_DNF;
	I2C1->CR1 |= 0x3 << I2C_CR1_DNF_Pos; // Set digital noise filter
	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH; 	 // Enable clock stretching
	I2C1->CR2 &= ~I2C_CR2_NACK;			 // Enable NACK generation
	I2C1->OAR1 = 0x0;
	I2C1->OAR2 = 0x0;
	I2C1->CR2 = 0x0;
	I2C1->CR1 |= I2C_CR1_PE;

	// Enable interrupts for I2C events
	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_SetPriority(I2C1_EV_IRQn, 2);
	I2C1->CR1 |= I2C_CR1_RXIE; // enable Receive Data Register not empty interrupt
}


bool i2c__detect(void) {
    uint8_t address;
    uint8_t i2c_timeout_ms = 100;
    bool detected = false;

    for (address = 0; address < 128; address++) {
        I2C1->CR2 = (address << 1) | I2C_CR2_RD_WRN; // Start I2C transmission for write operation
        I2C1->CR2 |= I2C_CR2_START; // Send START condition
        while (!(I2C1->ISR & I2C_ISR_TC) && (i2c_timeout_ms--)); // Wait for the end of transaction
        if (!(I2C1->ISR & I2C_ISR_NACKF)) { // Check if the device responded
        	detected = true;
            printf("slave_address: %i\n", address);
        }
        I2C1->CR2 |= I2C_CR2_STOP; // Generate STOP condition
        while ((I2C1->ISR & I2C_ISR_STOPF) && (i2c_timeout_ms--)); // Wait for the end of the transaction
        i2c_timeout_ms = 100;
    }
    return detected;
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
    I2C1->TXDR = register_address;                // Write the register address to the DR register
    while((I2C1->ISR & I2C_ISR_TXIS) == 0);  // Wait for TXIS flag to be set (transmit data)
    I2C1->TXDR = data; 						 // Write the data to the DR register
    while((I2C1->ISR & I2C_ISR_STOPF) == 0); // Wait for STOP flag to be set
    I2C1->ICR |= I2C_ICR_STOPCF;             // Clear the STOP flag by writing 0 to it
}

//void i2c1_event_irq_handler() {
//    if (I2C1->ISR & I2C_ISR_RXNE) {
//        i2c_rx_buffer[i2c_rx_index++] = I2C1->RXDR;
//    }
//}

//void i2c__HAL_read_slave_data(uint8_t deviceAddress, uint8_t* data, uint16_t dataSize) {
//    HAL_I2C_Master_Receive(&hi2c1, deviceAddress, data, dataSize, 1000);
//}


