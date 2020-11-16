#pragma once
#include <stdint.h>

extern volatile uint8_t response_byte;
// Call this function to initialize the SPI slave.
void spi_slave_init(void);

uint8_t spi_slave_get(void);
uint8_t spi_slave_available(void);