#ifndef AVR_GPIO_H
#define AVR_GPIO_H

#include <avr/io.h>

/** @name GPIO_GLOBAL_PULL_UP_CONTROL
 *  Public Functions to Control Global GPIO Pull Up
 */
/**@{*/
inline void GPIO_DisableGlobalPullUp() {
    PORTB &= ~(1 << PORTB7);
}

inline void GPIO_EnableGlobalPullUp() {
    PORTB |= (1 << PORTB7);
}
/**@}*/

/** @name GPIO_CONFIGURATION
 *  Public Functions to Configure GPIO as Digital I/O
 */
/**@{*/
inline void GPIO_Init(volatile uint8_t *Port, uint8_t Pin, uint8_t PinMode) {
    if (PinMode == 1) {
        *Port |= (1 << Pin);
    } else {
        *Port &= ~(1 << Pin);
    }
}

inline void GPIO_InitPin(uint8_t Pin, uint8_t PinMode) {
    if (PinMode == 1) {
        DDRB |= (1 << Pin);
    } else {
        DDRB &= ~(1 << Pin);
    }
}

inline void GPIO_DeInit(volatile uint8_t *Port, uint8_t Pin) {
    *Port &= ~(1 << Pin);
}

inline void GPIO_DeInitPin(uint8_t Pin) {
    DDRB &= ~(1 << Pin);
}
/**@}*/

/** @name GPIO_PIN_READ_WRITE
 *  Public Functions to Read and Write Digital I/O Pin
 */
/**@{*/
inline uint8_t GPIO_Read(volatile uint8_t *Port, uint8_t Pin) {
    return (*Port & (1 << Pin)) >> Pin;
}

inline uint8_t GPIO_ReadPin(uint8_t Pin) {
    return (PINB & (1 << Pin)) >> Pin;
}

inline void GPIO_Write(volatile uint8_t *Port, uint8_t Pin, uint8_t PinState) {
    if (PinState) {
        *Port |= (1 << Pin);
    } else {
        *Port &= ~(1 << Pin);
    }
}

inline void GPIO_Toggle(volatile uint8_t *Port, uint8_t Pin) {
    *Port ^= (1 << Pin);
}

inline void GPIO_WritePin(uint8_t Pin, uint8_t PinState) {
    if (PinState) {
        PORTB |= (1 << Pin);
    } else {
        PORTB &= ~(1 << Pin);
    }
}

inline void GPIO_WritePinLow(uint8_t Pin) {
    PORTB &= ~(1 << Pin);
}

inline void GPIO_WritePinHigh(uint8_t Pin) {
    PORTB |= (1 << Pin);
}

inline void GPIO_WritePinToggle(uint8_t Pin) {
    PORTB ^= (1 << Pin);
}
/**@}*/

#endif // AVR_GPIO_H
