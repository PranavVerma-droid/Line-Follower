#include <QTRSensors.h>

// Sensor pin definitions
#define QTR_1_PIN 2
#define QTR_2_PIN 4
#define QTR_3_PIN 7
#define QTR_4_PIN 8
#define QTR_5_PIN 9
#define QTR_6_PIN 10
#define QTR_7_PIN 12
#define QTR_8_PIN 13
#define QTR_EMITTER_PIN 1

// Motor pin definitions
#define MOTOR1A 5
#define MOTOR1B 6
#define MOTOR2A 3
#define MOTOR2B 11

#define QTR_MAX_SENSORS 8
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup()
{
  // Configure QTR sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){QTR_1_PIN, QTR_2_PIN, QTR_3_PIN, QTR_4_PIN, QTR_5_PIN, QTR_6_PIN, QTR_7_PIN, QTR_8_PIN}, SensorCount);
  // qtr.setEmitterPin(QTR_EMITTER_PIN);

  // Calibrate sensors
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++) { qtr.calibrate(); }
  digitalWrite(LED_BUILTIN, LOW);

  // Set motor pins as outputs
  pinMode(MOTOR1A, OUTPUT);
  pinMode(MOTOR1B, OUTPUT);
  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);

  // Initialize Serial
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  uint16_t position = qtr.readLineBlack(sensorValues);  // Get line position from sensors

  int leftSpeed = 150;
  int rightSpeed = 150;

  // Adjust speeds based on line position
  if (position < 2000)      // Line is towards the left
  {
    leftSpeed = 100;        // Slow down left motor
    rightSpeed = 200;       // Speed up right motor
  }
  else if (position > 3000) // Line is towards the right
  {
    leftSpeed = 200;        // Speed up left motor
    rightSpeed = 100;       // Slow down right motor
  }

  // Set motor speeds
  analogWrite(MOTOR1A, leftSpeed);  // Left motor forward
  analogWrite(MOTOR1B, 0);
  analogWrite(MOTOR2A, rightSpeed); // Right motor forward
  analogWrite(MOTOR2B, 0);

  delay(10);  // Short delay for stability
}
