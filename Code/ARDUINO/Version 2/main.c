#include <avr/io.h>
#include <util/delay.h>
#include "avr_gpio.h"
#include "avr_motor.h"
#include "avr_qtr.h"

#define LFR_MAX_MOTOR_SPEED 255
#define QTR_LINE_MID_VALUE 3200

void LFR_Initialize() {
    uint8_t QTR_Pins[] = {QTR_1_PIN, QTR_2_PIN, QTR_3_PIN, QTR_4_PIN, QTR_5_PIN, QTR_6_PIN, QTR_7_PIN, QTR_8_PIN};
    QTR_Init(QTR_Pins, QTR_EMITTER_PIN);
    Motor_Init();
    _delay_ms(2000);
}

void LFR_Calibrate() {
    Motor_SetSpeed(90, -90);
    for (uint8_t i = 0; i < 40; i++) {
        QTR_CalibrateSensor(QTR_EMITTERS_ON);
        _delay_ms(20);
    }
    Motor_SetSpeed(0, 0);
    _delay_ms(500);
    Motor_SetSpeed(-90, 90);
    for (uint8_t i = 0; i < 80; i++) {
        QTR_CalibrateSensor(QTR_EMITTERS_ON);
        _delay_ms(20);
    }
    Motor_SetSpeed(0, 0);
    _delay_ms(500);
    Motor_SetSpeed(90, -90);
    for (uint8_t i = 0; i < 40; i++) {
        QTR_CalibrateSensor(QTR_EMITTERS_ON);
        _delay_ms(20);
    }
    Motor_SetSpeed(0, 0);
    _delay_ms(2000);
}

int main() {
    uint16_t LFR_SensorValue[8];
    uint16_t LFR_Position = 0;
    int16_t LFR_Proportional = 0;
    int16_t LFR_LastProportional = 0;
    int16_t LFR_Derivative = 0;
    int64_t LFR_Integral = 0;
    int16_t LFR_ControlOutput = 0;

    LFR_Initialize();
    LFR_Calibrate();

    while (1) {
        LFR_Position = QTR_ReadLine(LFR_SensorValue, QTR_EMITTERS_ON);
        LFR_Proportional = LFR_Position - QTR_LINE_MID_VALUE;
        LFR_Derivative = LFR_Proportional - LFR_LastProportional;
        LFR_Integral += LFR_Proportional;
        LFR_LastProportional = LFR_Proportional;

        LFR_ControlOutput = (LFR_Proportional / 10) + (LFR_Integral / 10000) + (LFR_Derivative * 3 / 2);

        if (LFR_ControlOutput > LFR_MAX_MOTOR_SPEED) {
            LFR_ControlOutput = LFR_MAX_MOTOR_SPEED;
        }
        if (LFR_ControlOutput < -LFR_MAX_MOTOR_SPEED) {
            LFR_ControlOutput = -LFR_MAX_MOTOR_SPEED;
        }

        if (LFR_ControlOutput < 0) {
            Motor_SetSpeed(LFR_MAX_MOTOR_SPEED + LFR_ControlOutput, LFR_MAX_MOTOR_SPEED);
        } else {
            Motor_SetSpeed(LFR_MAX_MOTOR_SPEED, LFR_MAX_MOTOR_SPEED - LFR_ControlOutput);
        }
    }
}
