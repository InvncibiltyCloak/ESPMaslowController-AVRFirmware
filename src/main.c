#include "main.h"

int main() {
  sei();
  motors_init();
  spi_slave_init();

  while(1) {
    if (spi_slave_available() >= 4) {
      uint8_t address = spi_slave_get();
      response_byte = address;
      uint8_t value1 = spi_slave_get();
      uint8_t value2 = spi_slave_get();
      uint8_t value3 = spi_slave_get();

      switch (address) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          motors_set_channel(
            address,
            value1,
            value2 & 0x01
          );
      }
    }
  }
}
