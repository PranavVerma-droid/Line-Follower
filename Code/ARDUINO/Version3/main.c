#include <avr/io.h>
#include <util/delay.h>
#include "avr_gpio.h"
#include "avr_motor.h"
#include "avr_qtr.h"

#define F_CPU 16000000UL
#define LFR_MAX_MOTOR_SPEED 255

// PID constants tuned for 800 RPM
#define KP 0.1     // Proportional gain
#define KI 0.0001  // Integral gain
#define KD 1.5     // Derivative gain

void LFR_Initialize() {
    uint8_t QTR_Pins[] = {QTR_1_PIN, QTR_2_PIN, QTR_3_PIN, QTR_4_PIN, 
                          QTR_5_PIN, QTR_6_PIN, QTR_7_PIN, QTR_8_PIN};
    QTR_Init(QTR_Pins, QTR_EMITTER_PIN);
    Motor_Init();
    _delay_ms(2000);
}

void LFR_Calibrate() {
    // Calibration sequence with robot rotation
    Motor_SetSpeed(90, -90);
    for(uint8_t i = 0; i < 40; i++) {
        QTR_Calibrate(QTR_EMITTERS_ON);
        _delay_ms(20);
    }
    
    Motor_SetSpeed(0, 0);
    _delay_ms(500);
    
    Motor_SetSpeed(-90, 90);
    for(uint8_t i = 0; i < 80; i++) {
        QTR_Calibrate(QTR_EMITTERS_ON);
        _delay_ms(20);
    }
    
    Motor_SetSpeed(0, 0);
    _delay_ms(500);
    
    Motor_SetSpeed(90, -90);
    for(uint8_t i = 0; i < 40; i++) {
        QTR_Calibrate(QTR_EMITTERS_ON);
        _delay_ms(20);
    }
    
    Motor_SetSpeed(0, 0);
    _delay_ms(2000);
}

int main() {
    uint16_t sensorValues[QTR_MAX_SENSORS];
    uint16_t position = 0;
    int16_t lastError = 0;
    int32_t integral = 0;
    
    LFR_Initialize();
    LFR_Calibrate();
    
    while(1) {
        position = QTR_ReadLine(sensorValues, QTR_EMITTERS_ON);
        
        // PID calculations
        int16_t error = position - QTR_LINE_MID_VALUE;
        integral += error;
        int16_t derivative = error - lastError;
        lastError = error;
        
        // Calculate PID output
        int16_t motorSpeed = (KP * error) + (KI * integral) + (KD * derivative);
        
        // Apply motor speeds
        int16_t leftSpeed = LFR_MAX_MOTOR_SPEED;
        int16_t rightSpeed = LFR_MAX_MOTOR_SPEED;
        
        if(motorSpeed < 0) {
            leftSpeed += motorSpeed;
        } else {
            rightSpeed -= motorSpeed;
        }
        
        // Constrain speeds
        if(leftSpeed < 0) leftSpeed = 0;
        if(rightSpeed < 0) rightSpeed = 0;
        if(leftSpeed > LFR_MAX_MOTOR_SPEED) leftSpeed = LFR_MAX_MOTOR_SPEED;
        if(rightSpeed > LFR_MAX_MOTOR_SPEED) rightSpeed = LFR_MAX_MOTOR_SPEED;
        
        Motor_SetSpeed(leftSpeed, rightSpeed);
    }
    
    return 0;
}