#ifndef AVR_MOTOR_H_
#define AVR_MOTOR_H_

#include <avr/io.h>
#include "avr_gpio.h"

#define MOTOR1A 5 
#define MOTOR1B 6
#define MOTOR2A 3
#define MOTOR2B 11 

void Motor_Init(void) {
    GPIO_InitPin(MOTOR1A, OUTPUT);
    GPIO_InitPin(MOTOR1B, OUTPUT);
    GPIO_InitPin(MOTOR2A, OUTPUT);
    GPIO_InitPin(MOTOR2B, OUTPUT);

    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << CS11) | (1 << CS10);
}

void Motor_SetSpeed(int16_t M1Speed, int16_t M2Speed) {
    if(M1Speed >= 0) {
        GPIO_WritePin(MOTOR1A, 1);
        GPIO_WritePin(MOTOR1B, 0);
        OCR1A = M1Speed;
    }
    else {
        GPIO_WritePin(MOTOR1A, 0);
        GPIO_WritePin(MOTOR1B, 1);
        OCR1A = -M1Speed;
    }
    
    if(M2Speed >= 0) {
        GPIO_WritePin(MOTOR2A, 1);
        GPIO_WritePin(MOTOR2B, 0);
        OCR1B = M2Speed;
    }
    else {
        GPIO_WritePin(MOTOR2A, 0);
        GPIO_WritePin(MOTOR2B, 1);
        OCR1B = -M2Speed;
    }
}

#endif