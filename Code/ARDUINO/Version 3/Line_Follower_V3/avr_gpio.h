#ifndef AVR_GPIO_H_
#define AVR_GPIO_H_

#include <avr/io.h>

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

inline void GPIO_DisableGlobalPullUp() {
    MCUCR |= (1 << PUD);
}

inline void GPIO_EnableGlobalPullUp() {
    MCUCR &= ~(1 << PUD);
}

inline void GPIO_Init(volatile uint8_t *Port, uint8_t Pin, uint8_t PinMode) {
    if(PinMode == OUTPUT) {
        *(Port-1) |= (1 << Pin);
    }
    else {
        *(Port-1) &= ~(1 << Pin);
        if(PinMode == INPUT_PULLUP) {
            *Port |= (1 << Pin);
        }
    }
}

inline void GPIO_InitPin(uint8_t Pin, uint8_t PinMode) {
    volatile uint8_t *Port = &PORTB + (Pin/8);
    GPIO_Init(Port, Pin%8, PinMode);
}

inline void GPIO_DeInit(volatile uint8_t *Port, uint8_t Pin) {
    *(Port-1) &= ~(1 << Pin);
    *Port &= ~(1 << Pin);
}

inline void GPIO_DeInitPin(uint8_t Pin) {
    volatile uint8_t *Port = &PORTB + (Pin/8);
    GPIO_DeInit(Port, Pin%8);
}

inline uint8_t GPIO_Read(volatile uint8_t *Port, uint8_t Pin) {
    return (*(Port-2) & (1 << Pin)) != 0;
}

inline uint8_t GPIO_ReadPin(uint8_t Pin) {
    volatile uint8_t *Port = &PORTB + (Pin/8);
    return GPIO_Read(Port, Pin%8);
}

inline void GPIO_Write(volatile uint8_t *Port, uint8_t Pin, uint8_t PinState) {
    if(PinState) {
        *Port |= (1 << Pin);
    }
    else {
        *Port &= ~(1 << Pin);
    }
}

inline void GPIO_Toggle(volatile uint8_t *Port, uint8_t Pin) {
    *Port ^= (1 << Pin);
}

inline void GPIO_WritePin(uint8_t Pin, uint8_t PinState) {
    volatile uint8_t *Port = &PORTB + (Pin/8);
    GPIO_Write(Port, Pin%8, PinState);
}

#endif