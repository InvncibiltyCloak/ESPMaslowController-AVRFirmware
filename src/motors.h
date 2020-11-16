#pragma once
#include <stdint.h>

void motors_init(void);
void motors_set_channel(uint8_t channel, uint8_t pwm_value, uint8_t direction);