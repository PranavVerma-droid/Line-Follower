#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <Wire.h>
#include "avr_gpio.h"
#include "avr_motor.h"
#include "avr_qtr.h"
#include "avr_oled.h"

#define LFR_MAX_MOTOR_SPEED 200

#define KP 0.1
#define KI 0.0001
#define KD 1.5

AVR_OLED oled;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Line Follower...");

  uint8_t QTR_Pins[] = {QTR_1_PIN, QTR_2_PIN, QTR_3_PIN, QTR_4_PIN,
  QTR_5_PIN, QTR_6_PIN, QTR_7_PIN, QTR_8_PIN};
  QTR_Init(QTR_Pins, QTR_EMITTER_PIN);
  Motor_Init();
  _delay_ms(2000);

  Serial.println("Initializing OLED...");
  oled.init();
  oled.clearDisplay();
  oled.displayText("Line Follower", 0, 0);
  oled.displayText("Initialization Done", 0, 10);
  oled.display();
  _delay_ms(2000);

  Serial.println("Calibrating sensors...");
  LFR_Calibrate();
}

void LFR_Calibrate() {
  oled.clearDisplay();
  oled.displayText("Calibrating...", 0, 0);
  oled.display();

  Motor_SetSpeed(90, -90);
  for(uint8_t i = 0; i < 40; i++) {
    QTR_CalibrateSensor();
    _delay_ms(20);
  }

  Motor_SetSpeed(0, 0);
  _delay_ms(500);

  Motor_SetSpeed(-90, 90);
  for(uint8_t i = 0; i < 80; i++) {
    QTR_CalibrateSensor();
    _delay_ms(20);
  }

  Motor_SetSpeed(0, 0);
  _delay_ms(500);

  Motor_SetSpeed(90, -90);
  for(uint8_t i = 0; i < 40; i++) {
    QTR_CalibrateSensor();
    _delay_ms(20);
  }

  Motor_SetSpeed(0, 0);
  _delay_ms(2000);

  oled.clearDisplay();
  oled.displayText("Calibration Done", 0, 0);
  oled.display();
  _delay_ms(2000);

  Serial.println("Calibration Done");
}

void loop() {
  uint16_t sensorValues[QTR_MAX_SENSORS];
  uint16_t position = 0;
  int16_t lastError = 0;
  int32_t integral = 0;

  position = QTR_ReadLine(sensorValues);

  int16_t error = position - QTR_LINE_MID_VALUE;
  integral += error;
  int16_t derivative = error - lastError;
  lastError = error;

  int16_t motorSpeed = (KP * error) + (KI * integral) + (KD * derivative);

  int16_t leftSpeed = LFR_MAX_MOTOR_SPEED;
  int16_t rightSpeed = LFR_MAX_MOTOR_SPEED;

  if(motorSpeed < 0) {
    leftSpeed += motorSpeed;
  } else {
    rightSpeed -= motorSpeed;
  }

  if(leftSpeed < 0) leftSpeed = 0;
  if(rightSpeed < 0) rightSpeed = 0;
  if(leftSpeed > LFR_MAX_MOTOR_SPEED) leftSpeed = LFR_MAX_MOTOR_SPEED;
  if(rightSpeed > LFR_MAX_MOTOR_SPEED) rightSpeed = LFR_MAX_MOTOR_SPEED;

  Motor_SetSpeed(leftSpeed, rightSpeed);
  
  oled.clearDisplay();
  oled.displayText("Left Speed:", 0, 0);
  oled.displayText(String(leftSpeed).c_str(), 80, 0);
  oled.displayText("Right Speed:", 0, 10);
  oled.displayText(String(rightSpeed).c_str(), 80, 10);
  oled.display();

  Serial.print("Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" Right Speed: ");
  Serial.println(rightSpeed);
}
