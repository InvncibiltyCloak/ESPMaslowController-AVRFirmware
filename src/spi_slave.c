#include "spi_slave.h"

#include "pinspec.h"
#include "gpio_ops.h"
#include <avr/interrupt.h>

static volatile uint8_t byte_num;
volatile uint8_t response_byte;

// Circular buffer for RX
// Code from https://www.embeddedrelated.com/showthread/comp.arch.embedded/77084-1.php
#define BUF_SIZE 64
volatile uint8_t head, tail;
volatile uint8_t buffer[BUF_SIZE];
static inline uint8_t inuse(void) { return (head + BUF_SIZE - tail) % BUF_SIZE; }
static inline void put(uint8_t data) { if (inuse() != BUF_SIZE) { buffer[head++%BUF_SIZE] = data; } }
static inline uint8_t get(void) { if (inuse() != 0) { return buffer[tail++%BUF_SIZE]; } return 0; }


void spi_slave_init(void) {
    // Setup the GPIO pin directions
    SET_PIN_OUTPUT(PIN_SPI_AVR_MISO);
    SET_PIN_INPUT(PIN_SPI_AVR_MOSI);
    SET_PIN_INPUT(PIN_SPI_AVR_SCLK);
    SET_PIN_INPUT(PIN_SPI_AVR_CS0);

    // Setup SPI0 as slave
    SPCR0 = _BV(SPIE) | _BV(SPE); // Enable the SPI port and interrupt in slave mode
    PCMSK0 = _BV(2);              // Setup pin change interrupt for SS pin
    PCICR = _BV(PCIE0);
}

uint8_t spi_slave_get(void) {
  return get();
}

uint8_t spi_slave_available(void) {
  return inuse();
}


// SPI bus interrupt handler
ISR(SPI0_STC_vect) {
  if(byte_num < 4) {
    put(SPDR0);
  }
  byte_num += 1;
  SPDR0 = response_byte;
}

// Interrupt Handler for SS for SPI bus
ISR(PCINT0_vect) {
  if(!PIN_READ(PIN_SPI_AVR_CS0)) {
    // SPI selected
    byte_num = 0;
  }
}
