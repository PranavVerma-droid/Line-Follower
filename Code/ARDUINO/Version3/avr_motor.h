#ifndef AVR_MOTOR_H
#define AVR_MOTOR_H

#include <avr/io.h>
#include "avr_gpio.h"

#define MOTOR_1A_PORT PORTD
#define MOTOR_1B_PORT PORTD
#define MOTOR_2A_PORT PORTD
#define MOTOR_2B_PORT PORTD

#define MOTOR_1A_PIN 5  // PWM capable pin
#define MOTOR_1B_PIN 6  // PWM capable pin
#define MOTOR_2A_PIN 9  // PWM capable pin
#define MOTOR_2B_PIN 10 // PWM capable pin

void Motor_Init() {
    // Configure PWM pins
    TCCR0A |= (1 << WGM00) | (1 << WGM01); // Fast PWM mode
    TCCR0B |= (1 << CS01);  // Prescaler = 8 for 800 RPM
    
    // Set motor pins as outputs
    GPIO_Init(&MOTOR_1A_PORT, MOTOR_1A_PIN, GPIO_OUTPUT);
    GPIO_Init(&MOTOR_1B_PORT, MOTOR_1B_PIN, GPIO_OUTPUT);
    GPIO_Init(&MOTOR_2A_PORT, MOTOR_2A_PIN, GPIO_OUTPUT);
    GPIO_Init(&MOTOR_2B_PORT, MOTOR_2B_PIN, GPIO_OUTPUT);
}

void Motor_SetSpeed(int16_t M1Speed, int16_t M2Speed) {
    // Motor 1 control
    if(M1Speed >= 0) {
        OCR0A = M1Speed > 255 ? 255 : M1Speed;
        GPIO_Write(&MOTOR_1A_PORT, MOTOR_1A_PIN, GPIO_HIGH);
        GPIO_Write(&MOTOR_1B_PORT, MOTOR_1B_PIN, GPIO_LOW);
    } else {
        OCR0A = (-M1Speed) > 255 ? 255 : -M1Speed;
        GPIO_Write(&MOTOR_1A_PORT, MOTOR_1A_PIN, GPIO_LOW);
        GPIO_Write(&MOTOR_1B_PORT, MOTOR_1B_PIN, GPIO_HIGH);
    }
    
    // Motor 2 control
    if(M2Speed >= 0) {
        OCR0B = M2Speed > 255 ? 255 : M2Speed;
        GPIO_Write(&MOTOR_2A_PORT, MOTOR_2A_PIN, GPIO_HIGH);
        GPIO_Write(&MOTOR_2B_PORT, MOTOR_2B_PIN, GPIO_LOW);
    } else {
        OCR0B = (-M2Speed) > 255 ? 255 : -M2Speed;
        GPIO_Write(&MOTOR_2A_PORT, MOTOR_2A_PIN, GPIO_LOW);
        GPIO_Write(&MOTOR_2B_PORT, MOTOR_2B_PIN, GPIO_HIGH);
    }
}