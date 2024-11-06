#ifndef AVR_MOTOR_H
#define AVR_MOTOR_H

#include "avr_gpio.h"

/** @name MOTOR_CONTROL
 *  Public Functions to Control Motor
 */
/**@{*/
void Motor_Init(void) {
    GPIO_InitPin(M1_PIN, 1);  // Set M1_PIN as output
    GPIO_InitPin(M2_PIN, 1);  // Set M2_PIN as output
}

void Motor_DeInit(void) {
    GPIO_DeInitPin(M1_PIN);   // Set M1_PIN as input
    GPIO_DeInitPin(M2_PIN);   // Set M2_PIN as input
}

void Motor_SetSpeed(int16_t M1Speed, int16_t M2Speed) {
    if (M1Speed > 0) {
        GPIO_WritePinHigh(M1_PIN);
        GPIO_WritePinLow(M1_BRK_PIN);
    } else if (M1Speed < 0) {
        GPIO_WritePinLow(M1_PIN);
        GPIO_WritePinHigh(M1_BRK_PIN);
    } else {
        GPIO_WritePinLow(M1_PIN);
        GPIO_WritePinLow(M1_BRK_PIN);
    }

    if (M2Speed > 0) {
        GPIO_WritePinHigh(M2_PIN);
        GPIO_WritePinLow(M2_BRK_PIN);
    } else if (M2Speed < 0) {
        GPIO_WritePinLow(M2_PIN);
        GPIO_WritePinHigh(M2_BRK_PIN);
    } else {
        GPIO_WritePinLow(M2_PIN);
        GPIO_WritePinLow(M2_BRK_PIN);
    }
}

void Motor_SetSpeedM1(int16_t M1Speed) {
    if (M1Speed > 0) {
        GPIO_WritePinHigh(M1_PIN);
        GPIO_WritePinLow(M1_BRK_PIN);
    } else if (M1Speed < 0) {
        GPIO_WritePinLow(M1_PIN);
        GPIO_WritePinHigh(M1_BRK_PIN);
    } else {
        GPIO_WritePinLow(M1_PIN);
        GPIO_WritePinLow(M1_BRK_PIN);
    }
}

void Motor_SetSpeedM2(int16_t M2Speed) {
    if (M2Speed > 0) {
        GPIO_WritePinHigh(M2_PIN);
        GPIO_WritePinLow(M2_BRK_PIN);
    } else if (M2Speed < 0) {
        GPIO_WritePinLow(M2_PIN);
        GPIO_WritePinHigh(M2_BRK_PIN);
    } else {
        GPIO_WritePinLow(M2_PIN);
        GPIO_WritePinLow(M2_BRK_PIN);
    }
}
/**@}*/

#endif // AVR_MOTOR_H
