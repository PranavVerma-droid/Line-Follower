#ifndef AVR_QTR_H_
#define AVR_QTR_H_

#include <avr/io.h>
#include "avr_gpio.h"

#define QTR_1_PIN 2
#define QTR_2_PIN 4
#define QTR_3_PIN 7
#define QTR_4_PIN 8
#define QTR_5_PIN 9
#define QTR_6_PIN 10
#define QTR_7_PIN 12
#define QTR_8_PIN 13
#define QTR_EMITTER_PIN 1

#define QTR_MAX_SENSORS 8
#define QTR_TIMEOUT 2500
#define QTR_EMITTERS_ON 1
#define QTR_EMITTERS_OFF 0
#define QTR_LINE_MID_VALUE 3500

static uint8_t QTRSensorCount;
static uint8_t *QTRSensorPins;
static uint8_t QTREmitterPin;
static uint16_t QTRCalibrationMin[QTR_MAX_SENSORS];
static uint16_t QTRCalibrationMax[QTR_MAX_SENSORS];

void QTR_Init(uint8_t *SensorPins, uint8_t EmitterPin) {
    QTRSensorPins = SensorPins;
    QTREmitterPin = EmitterPin;
    QTRSensorCount = QTR_MAX_SENSORS;
    
    GPIO_InitPin(QTREmitterPin, OUTPUT);
    
    for(uint8_t i = 0; i < QTRSensorCount; i++) {
        GPIO_InitPin(QTRSensorPins[i], INPUT);
    }
}

void QTR_EmitterOn() {
    GPIO_WritePin(QTREmitterPin, 1);
}

void QTR_EmitterOff() {
    GPIO_WritePin(QTREmitterPin, 0);
}

void QTR_ReadRaw(uint16_t *SensorValues) {
    for(uint8_t i = 0; i < QTRSensorCount; i++) {
        GPIO_InitPin(QTRSensorPins[i], OUTPUT);
        GPIO_WritePin(QTRSensorPins[i], 1);
    }
    
    _delay_us(10);
    
    for(uint8_t i = 0; i < QTRSensorCount; i++) {
        GPIO_InitPin(QTRSensorPins[i], INPUT);
    }
    
    uint16_t startTime = TCNT1;
    
    while(TCNT1 - startTime < QTR_TIMEOUT) {
        uint8_t done = 1;
        for(uint8_t i = 0; i < QTRSensorCount; i++) {
            if(GPIO_ReadPin(QTRSensorPins[i]) && SensorValues[i] == 0) {
                SensorValues[i] = TCNT1 - startTime;
                done &= 0;
            }
        }
        if(done) break;
    }
}

void QTR_ReadCalibrated(uint16_t *SensorValues) {
    uint16_t rawValues[QTR_MAX_SENSORS];
    QTR_ReadRaw(rawValues);
    
    for(uint8_t i = 0; i < QTRSensorCount; i++) {
        uint16_t calRange = QTRCalibrationMax[i] - QTRCalibrationMin[i];
        int16_t value = 0;
        
        if(rawValues[i] > QTRCalibrationMax[i])
            value = 1000;
        else if(rawValues[i] < QTRCalibrationMin[i])
            value = 0;
        else
            value = (((int32_t)rawValues[i] - QTRCalibrationMin[i]) * 1000) / calRange;
            
        SensorValues[i] = value;
    }
}

int16_t QTR_ReadLine(uint16_t *SensorValues) {
    QTR_ReadCalibrated(SensorValues);
    
    uint32_t avg = 0;
    uint32_t sum = 0;
    
    for(uint8_t i = 0; i < QTRSensorCount; i++) {
        uint16_t value = SensorValues[i];
        avg += (uint32_t)value * (i * 1000);
        sum += value;
    }
    
    return sum != 0 ? avg / sum : QTR_LINE_MID_VALUE;
}

void QTR_CalibrateSensor() {
    uint16_t sensorValues[QTR_MAX_SENSORS];
    QTR_ReadRaw(sensorValues);
    
    for(uint8_t i = 0; i < QTRSensorCount; i++) {
        if(sensorValues[i] < QTRCalibrationMin[i])
            QTRCalibrationMin[i] = sensorValues[i];
        if(sensorValues[i] > QTRCalibrationMax[i])
            QTRCalibrationMax[i] = sensorValues[i];
    }
}

#endif