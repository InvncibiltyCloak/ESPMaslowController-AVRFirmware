#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN_PWM1 D, 1
#define PIN_PWM2 D, 2
#define PIN_PWM3 D, 3
#define PIN_PWM4 B, 1
#define PIN_PWM5 D, 0
#define PIN_PWM6 D, 5
#define PIN_PWM7 D, 6

#define PIN_DIR1 E, 1
#define PIN_DIR2 E, 0
#define PIN_DIR3 D, 4
#define PIN_DIR4 C, 2
#define PIN_DIR5 B, 0

#define PIN_SPI_AVR_MISO B, 4
#define PIN_SPI_AVR_MOSI B, 3
#define PIN_SPI_AVR_SCLK B, 5
#define PIN_SPI_AVR_CS0  B, 2

#define PIN_SPI_MOT_MISO C, 0
#define PIN_SPI_MOT_MOSI E, 3
#define PIN_SPI_MOT_SCLK C, 1
#define PIN_SPI_MOT_CS0  E, 2

#define _SET_PIN_OUTPUT(PORT, PIN) (DDR##PORT |= _BV(PIN))
#define SET_PIN_OUTPUT(PINSPEC)    _SET_PIN_OUTPUT(PINSPEC)

#define _SET_PIN_INPUT(PORT, PIN) (DDR##PORT &= ~_BV(PIN))
#define SET_PIN_INPUT(PINSPEC)    _SET_PIN_OUTPUT(PINSPEC)

#define _PIN_OUTPUT_HIGH(PORT, PIN) (PORT##PORT |= _BV(PIN))
#define PIN_OUTPUT_HIGH(PINSPEC)    _OUTPUT_HIGH(PINSPEC)

#define _PIN_OUTPUT_LOW(PORT, PIN) (PORT##PORT &= ~_BV(PIN))
#define PIN_OUTPUT_LOW(PINSPEC)    _OUTPUT_LOW(PINSPEC)

void setup_timer_4A(void);
void setup_spi_slave_0(void);

int main() {
  sei();

  SET_PIN_OUTPUT(PIN_DIR1);
  SET_PIN_OUTPUT(PIN_PWM1);
  setup_timer_4A();

  SET_PIN_OUTPUT(PIN_SPI_AVR_MISO);
  SET_PIN_INPUT(PIN_SPI_AVR_MOSI);
  SET_PIN_INPUT(PIN_SPI_AVR_SCLK);
  SET_PIN_INPUT(PIN_SPI_AVR_CS0);
  setup_spi_slave_0();

}

void setup_timer_4A(void) {
  TCCR4A = 2 << COM4A0 | 1 << WGM40; // Enable OC4A output and set to phase-accurate PWM
  TCCR4B = 0 << WGM42 | 3 << CS40; // Set phase-accurate PWM and start clock at divider 64
  OCR4A = 1;
}

void setup_spi_slave_0(void) {
  SPCR0 = _BV(SPIE) | _BV(SPE); // Enable the SPI port and interrupt in slave mode
}

ISR(SPI0_STC_vect) {
  OCR4A = SPDR0;
}