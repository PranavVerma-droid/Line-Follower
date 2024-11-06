#ifndef AVR_QTR_H
#define AVR_QTR_H

#include "avr_gpio.h"

/** @name QTR_CONTROL
 *  Public Functions to Control QTR Sensor
 */
/**@{*/
void QTR_Init(uint8_t *SensorPin, uint8_t EmitterPin) {
    for (uint8_t i = 0; i < 8; i++) {
        GPIO_InitPin(SensorPin[i], 0);  // Set sensor pins as input
    }
    GPIO_InitPin(EmitterPin, 1);       // Set emitter pin as output
}

void QTR_DeInit() {
    for (uint8_t i = 0; i < 8; i++) {
        GPIO_DeInitPin(QTR_PINS[i]);    // Set sensor pins as input
    }
    GPIO_DeInitPin(QTR_EMITTER_PIN);   // Set emitter pin as input
}

void QTR_Calibrate(uint16_t *CalibratedMinimum, uint16_t *CalibratedMaximum, uint8_t ReadMode) {
    uint16_t sensorValues[8];
    for (uint8_t i = 0; i < 8; i++) {
        sensorValues[i] = 0;
        for (uint8_t j = 0; j < 10; j++) {
            sensorValues[i] += QTR_ReadRaw(sensorValues);
        }
        sensorValues[i] /= 10;
    }
    for (uint8_t i = 0; i < 8; i++) {
        CalibratedMinimum[i] = sensorValues[i];
        CalibratedMaximum[i] = sensorValues[i];
    }
}

void QTR_CalibrateSensor(uint8_t ReadMode) {
    GPIO_WritePinHigh(QTR_EMITTER_PIN);
    delay_ms(1);
    uint16_t sensorValues[8];
    for (uint8_t i = 0; i < 8; i++) {
        sensorValues[i] = QTR_ReadRaw(sensorValues);
    }
    GPIO_WritePinLow(QTR_EMITTER_PIN);
}

void QTR_ResetCalibration() {
    // Reset calibration values
}

void QTR_ReadSensor(uint16_t *SensorValues, uint8_t ReadMode) {
    for (uint8_t i = 0; i < 8; i++) {
        SensorValues[i] = QTR_ReadRaw(SensorValues);
    }
}

void QTR_ReadRaw(uint16_t *SensorValues) {
    for (uint8_t i = 0; i < 8; i++) {
        SensorValues[i] = ADC_Read(QTR_PINS[i]);
    }
}

void QTR_ReadCalibrated(uint16_t *SensorValues, uint8_t ReadMode) {
    // Read calibrated sensor values
}

int16_t QTR_ReadLine(uint16_t *SensorValues, uint8_t ReadMode) {
    int16_t linePosition = 0;
    for (uint8_t i = 0; i < 8; i++) {
        linePosition += (i * SensorValues[i]);
    }
    return linePosition;
}

void QTR_EmitterOn() {
    GPIO_WritePinHigh(QTR_EMITTER_PIN);
}

void QTR_EmitterOff() {
    GPIO_WritePinLow(QTR_EMITTER_PIN);
}
/**@}*/

#endif // AVR_QTR_H
