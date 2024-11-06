#ifndef AVR_QTR_H
#define AVR_QTR_H

#include <avr/io.h>
#include "avr_gpio.h"

#define QTR_MAX_SENSORS 8
#define QTR_TIMEOUT 2500
#define QTR_LINE_MID_VALUE 3500

#define QTR_EMITTERS_OFF 0
#define QTR_EMITTERS_ON 1

// QTR sensor pins based on circuit diagram
#define QTR_1_PIN 2
#define QTR_2_PIN 3
#define QTR_3_PIN 4
#define QTR_4_PIN 5
#define QTR_5_PIN 6
#define QTR_6_PIN 7
#define QTR_7_PIN 8
#define QTR_8_PIN 9
#define QTR_EMITTER_PIN 11

static uint16_t qtr_calibratedMin[QTR_MAX_SENSORS];
static uint16_t qtr_calibratedMax[QTR_MAX_SENSORS];
static uint8_t *qtr_pins;
static uint8_t qtr_emitterPin;

void QTR_Init(uint8_t *sensorPins, uint8_t emitterPin) {
    qtr_pins = sensorPins;
    qtr_emitterPin = emitterPin;
    
    // Initialize pins
    for(uint8_t i = 0; i < QTR_MAX_SENSORS; i++) {
        GPIO_Init(&PORTD, qtr_pins[i], GPIO_INPUT);
        qtr_calibratedMin[i] = QTR_TIMEOUT;
        qtr_calibratedMax[i] = 0;
    }
    
    GPIO_Init(&PORTD, qtr_emitterPin, GPIO_OUTPUT);
}

void QTR_Calibrate(uint8_t readMode) {
    if(readMode == QTR_EMITTERS_ON) {
        GPIO_Write(&PORTD, qtr_emitterPin, GPIO_HIGH);
    }
    
    for(uint8_t i = 0; i < QTR_MAX_SENSORS; i++) {
        GPIO_Init(&PORTD, qtr_pins[i], GPIO_OUTPUT);
        GPIO_Write(&PORTD, qtr_pins[i], GPIO_HIGH);
        
        // Charge capacitor
        _delay_us(10);
        
        GPIO_Init(&PORTD, qtr_pins[i], GPIO_INPUT);
        
        // Time the discharge
        uint16_t time = 0;
        while(GPIO_Read(&PORTD, qtr_pins[i]) && time < QTR_TIMEOUT) {
            time++;
            _delay_us(1);
        }
        
        // Update calibration values
        if(time < qtr_calibratedMin[i]) {
            qtr_calibratedMin[i] = time;
        }
        if(time > qtr_calibratedMax[i]) {
            qtr_calibratedMax[i] = time;
        }
    }
    
    if(readMode == QTR_EMITTERS_ON) {
        GPIO_Write(&PORTD, qtr_emitterPin, GPIO_LOW);
    }
}

int16_t QTR_ReadLine(uint16_t *sensorValues, uint8_t readMode) {
    uint32_t avg = 0;
    uint32_t sum = 0;
    
    for(uint8_t i = 0; i < QTR_MAX_SENSORS; i++) {
        uint16_t value = sensorValues[i];
        
        // Map sensor value to 0-1000 range
        if(value < qtr_calibratedMin[i])
            value = qtr_calibratedMin[i];
        if(value > qtr_calibratedMax[i])
            value = qtr_calibratedMax[i];
            
        value = ((uint32_t)(value - qtr_calibratedMin[i]) * 1000) / 
                (qtr_calibratedMax[i] - qtr_calibratedMin[i]);
        
        avg += (uint32_t)value * i * 1000;
        sum += value;
    }
    
    return sum ? (int16_t)(avg / sum) : QTR_LINE_MID_VALUE;
}