#include <stdint.h>
#include <stdbool.h>

#define NO_CHAR_AVAILABLE ((char)0xff)

enum
{
	DRIVER_UART1 = 0,
	DRIVER_UART2,
	DRIVER_UART3,

	NUM_UART_DRIVERS
};

bool uart__HAL_initialize(uint8_t id, uint32_t baudrate);
void uart__HAL_send_chars(uint8_t id, uint8_t *c, uint16_t size);
char uart__HAL_get_char(uint8_t id);

bool uart__initialize(uint8_t id, uint32_t baudrate);
void uart__put_tx(uint8_t id, uint8_t *buff, uint16_t size);
uint8_t uart__get_char(uint8_t id);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
