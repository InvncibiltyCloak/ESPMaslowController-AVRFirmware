#include "motors.h"
#include "pinspec.h"
#include "gpio_ops.h"

// -------- STATIC DECLARATIONS --------
static inline void setup_timer_0(void);
static inline void setup_timer_1A(void);
static inline void setup_timer_2B(void);
static inline void setup_timer_3(void);
static inline void setup_timer_4A(void);

// Channel 1 -> OC4A
// Channel 2 -> OC3B
// Channel 3 -> OC2B
// Channel 4 -> OC1A
// Channel 5 -> OC3A
// Channel 6 -> OC0B
// Channel 7 -> OC0A

void motors_init(void) {
    SET_PIN_OUTPUT(PIN_DIR1);
    SET_PIN_OUTPUT(PIN_PWM1);
    SET_PIN_OUTPUT(PIN_DIR2);
    SET_PIN_OUTPUT(PIN_PWM2);
    SET_PIN_OUTPUT(PIN_DIR3);
    SET_PIN_OUTPUT(PIN_PWM3);
    SET_PIN_OUTPUT(PIN_DIR4);
    SET_PIN_OUTPUT(PIN_PWM4);
    SET_PIN_OUTPUT(PIN_DIR5);
    SET_PIN_OUTPUT(PIN_PWM5);

    // PWM 6 and 7 have no associated direction pin
    // In fact they are intended to be AUX pins, not necessarily motors
    // But we deal with them here anyway
    SET_PIN_OUTPUT(PIN_PWM6);
    SET_PIN_OUTPUT(PIN_PWM7);

    // Setup the timers for the PWMs
    setup_timer_0();
    setup_timer_1A();
    setup_timer_2B();
    setup_timer_3();
    setup_timer_4A();
}


void motors_set_channel(uint8_t channel, uint8_t pwm_value, uint8_t direction) {
    // Set the PWM value and corresponding direction pin, if it exists
    switch (channel) {
        case 1:
            OCR4A = pwm_value;
            PIN_OUTPUT_VALUE(PIN_DIR1, direction);
            break;
        case 2:
            OCR3B = pwm_value;
            PIN_OUTPUT_VALUE(PIN_DIR2, direction);
            break;
        case 3:
            OCR2B = pwm_value;
            PIN_OUTPUT_VALUE(PIN_DIR3, direction);
            break;
        case 4:
            OCR1A = pwm_value;
            PIN_OUTPUT_VALUE(PIN_DIR4, direction);
            break;
        case 5:
            OCR3A = pwm_value;
            PIN_OUTPUT_VALUE(PIN_DIR5, direction);
            break;
        case 6:
            OCR0B = pwm_value;
            break;
        case 7:
            OCR0A = pwm_value;
            break;
    }
    
}

static inline void setup_timer_4A(void) {
    TCCR4A = 2 << COM4A0 | 1 << WGM40; // Enable OC4A output and set to phase-accurate PWM
    TCCR4B = 3 << CS40; // Set phase-accurate PWM and start clock at divider 64
    OCR4A = 0;
}

static inline void setup_timer_3(void) {
    PIN_OUTPUT_HIGH(PIN_PWM2); // OC3B has the OCM on it, so PORTD.1 must be high for OR operation
    TCCR3A = 2 << COM3A0 | 2 << COM3B0 | 1 << WGM30; // Enable outputs and set to phase-accurate PWM
    TCCR3B = 0 << WGM32 | 3 << CS30; // Set phase-accurate PWM and start clock at divider 64
    OCR3A = 0;
    OCR3B = 0;
}

static inline void setup_timer_2B(void) {
    TCCR2A = 2 << COM2B0 | 1 << WGM20; // Enable OC2B output and set to phase-accurate PWM
    TCCR2B = 0 << WGM22 | 3 << CS20; // Set phase-accurate PWM and start clock at divider 64
    OCR2B = 0;
}

static inline void setup_timer_1A(void) {
    TCCR1A = 2 << COM1A0 | 1 << WGM10; // Enable OC1A output and set to phase-accurate PWM
    TCCR1B = 0 << WGM12 | 3 << CS10; // Set phase-accurate PWM and start clock at divider 64
    OCR1A = 0;
}

static inline void setup_timer_0(void) {
    TCCR0A = 2 << COM0A0 | 2 << COM0B0 | 1 << WGM00; // Enable outputs and set to phase-accurate PWM
    TCCR0B = 0 << WGM02 | 3 << CS00; // Set phase-accurate PWM and start clock at divider 64
    OCR0A = 0;
    OCR0B = 0;
}