#ifndef AVR_GPIO_H
#define AVR_GPIO_H

#include <avr/io.h>

// Pin mode definitions
#define GPIO_INPUT 0
#define GPIO_OUTPUT 1
#define GPIO_INPUT_PULLUP 2

// Pin state definitions
#define GPIO_LOW 0
#define GPIO_HIGH 1

// GPIO control functions
inline void GPIO_DisableGlobalPullUp() {
    MCUCR |= (1 << PUD);
}

inline void GPIO_EnableGlobalPullUp() {
    MCUCR &= ~(1 << PUD);
}

inline void GPIO_Init(volatile uint8_t *Port, uint8_t Pin, uint8_t PinMode) {
    if(PinMode == GPIO_OUTPUT) {
        *(Port - 1) |= (1 << Pin);  // Set DDRx register
    } else {
        *(Port - 1) &= ~(1 << Pin); // Clear DDRx register
        if(PinMode == GPIO_INPUT_PULLUP) {
            *Port |= (1 << Pin);     // Enable pull-up
        }
    }
}

inline void GPIO_Write(volatile uint8_t *Port, uint8_t Pin, uint8_t PinState) {
    if(PinState) {
        *Port |= (1 << Pin);
    } else {
        *Port &= ~(1 << Pin);
    }
}

inline uint8_t GPIO_Read(volatile uint8_t *Port, uint8_t Pin) {
    return (*Port & (1 << Pin)) ? 1 : 0;
}