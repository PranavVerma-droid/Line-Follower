#include <QTRSensors.h>
#include <U8g2lib.h>

// Motor A - Left Motor
#define AIN1 8  // Direction pin 1
#define AIN2 7  // Direction pin 2
#define PWMA 5  // PWM speed control
// Motor B - Right Motor
#define BIN1 9  // Direction pin 1
#define BIN2 10 // Direction pin 2
#define PWMB 6  // PWM speed control
#define STBY 4  // Standby pin

void setupMotors();
void setMotorSpeed(int leftSpeed, int rightSpeed);

// QTR Sensor configuration - using ANALOG pins
#define NUM_SENSORS 8
#define EMITTER_PIN 2
QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];
void setupSensorArray();


#define OLED_SDA 11
#define OLED_SCL 12
// U8g2 constructor for 128x32 OLED using software I2C
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(
  U8G2_R0,    // Rotation: no rotation
  OLED_SCL,   // Clock pin
  OLED_SDA,   // Data pin
  U8X8_PIN_NONE // Reset pin (not used)
);
void setupOLED();
void updateDisplay(uint16_t position);


void performManualCalibration();

/*float Kp = 0.25;    // Slightly increase Kp to be more responsive on sharper curves
float Ki = 0.002;     // Still no Ki for now
float Kd = 0.21;    // Increase Kd to improve sharp curve anticipation */

/*
float Kp = 0.5;   // Proportional, much softer
float Ki = 0.04;  // Slightly higher integral
float Kd = 0.2;   // Much lower derivative */


float Kp = 0.26;
float Ki = 0.15;
float Kd = 0.16;

/* float Kp = 0.3;   // Reduced proportional for less immediate response
float Ki = 0.02;  // Lower integral to reduce oscillation buildup
float Kd = 0.1;   // Halved derivative for smoother response */


int maxSpeed = 220;      // Maximum motor speed (0-255)
int baseSpeed = 180;     // Base speed for straight line


// PID variables
int lastError = 0;
long integral = 0;
int setPoint = ((NUM_SENSORS - 1) * 1000) / 2; // Center position (3500 for 8 sensors)

// Motor speed variable

// Manual calibration time in milliseconds (30 seconds)
unsigned long calibrationTime = 30000;

void setup() {
	setupMotors();
	setupOLED();
	Serial.begin(9600);
	setupSensorArray();
	performManualCalibration();
  delay(1000);
}

void loop() {
	// setMotorSpeed(200, 200);
	uint16_t position = qtr.readLineBlack(sensorValues);
	updateDisplay(position);

	int error = position - setPoint;

  // Calculate PID terms
	integral += error;
	// Avoid integral windup
	integral = constrain(integral, -10000, 10000);
	
	int derivative = error - lastError;
	lastError = error;
	
	// Calculate motor adjustment
	
	int motorSpeed = Kp * error + Ki * integral + Kd * derivative;
	if (abs(error) < 250) { motorSpeed = 0; }
	// Set motor speeds - Modified to prevent backward motion
	int leftMotorSpeed = baseSpeed - motorSpeed ;
	int rightMotorSpeed = baseSpeed + motorSpeed;
	
	// Constrain speeds to valid range (0-maxSpeed)
	// Changed from -maxSpeed to 0 to prevent backward motion
	leftMotorSpeed = constrain(leftMotorSpeed, 0, maxSpeed);
	rightMotorSpeed = constrain(rightMotorSpeed, 0, maxSpeed);
	
	// Set motor speeds
	setMotorSpeed(leftMotorSpeed, rightMotorSpeed);
	
	// Update OLED display
	updateDisplay(position);
}

void setupSensorArray() {
	qtr.setTypeAnalog();
  	qtr.setSensorPins((const uint8_t[]){A7, A6, A5, A4, A3, A2, A1, A0}, NUM_SENSORS);
  	qtr.setEmitterPin(EMITTER_PIN);
}

void setupMotors() {
	Serial.println("Motor Driver Test - Starting...");
	pinMode(AIN1, OUTPUT);
	pinMode(AIN2, OUTPUT);
	pinMode(PWMA, OUTPUT);
	pinMode(BIN1, OUTPUT);
	pinMode(BIN2, OUTPUT);
	pinMode(PWMB, OUTPUT);
	pinMode(STBY, OUTPUT);
	digitalWrite(STBY, HIGH);

	digitalWrite(AIN1, HIGH);
    	digitalWrite(AIN2, LOW);

	digitalWrite(BIN1, HIGH);
    	digitalWrite(BIN2, LOW);
	Serial.println("Motor driver enabled (STBY HIGH)");
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
  static int lastLeftSpeed = 0;
  static int lastRightSpeed = 0;

  // Set final PWM normally
  analogWrite(PWMA, leftSpeed);
  analogWrite(PWMB, rightSpeed);

  lastLeftSpeed = leftSpeed;
  lastRightSpeed = rightSpeed;

  // Debug output
  Serial.print("Motor speeds set - Left: ");
  Serial.print(leftSpeed);
  Serial.print(", Right: ");
  Serial.println(rightSpeed);
}

void setupOLED() {
	u8g2.begin();
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_6x10_tf);
	u8g2.drawStr(0, 10, "Line Follower Robot");
	u8g2.drawStr(0, 20, "Initializing...");
	u8g2.sendBuffer();
}

void updateDisplay(uint16_t position) {
	u8g2.clearBuffer();
	
	// Display position reading
	u8g2.setFont(u8g2_font_6x10_tf);
	u8g2.setCursor(0, 8);
	u8g2.print("Pos: ");
	u8g2.print(position);

	int lineX = map(position, 0, (NUM_SENSORS - 1) * 1000, 0, 88);
	u8g2.drawFrame(40, 10, 88, 6);  // Draw frame for position
	u8g2.drawBox(40 + lineX, 10, 5, 6);  // Draw position indicator
	
	// Draw sensor value bars
	for (int i = 0; i < NUM_SENSORS; i++) {
	int barHeight = map(sensorValues[i], 0, 1000, 0, 9);
	u8g2.drawBox(i * 16, 32 - barHeight, 14, barHeight);
	}
	u8g2.sendBuffer();
}

void performManualCalibration() {
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_6x10_tf);
	u8g2.drawStr(0, 10, "QTR Calibration");
	u8g2.drawStr(0, 20, "Move sensors over");
	u8g2.drawStr(0, 30, "line for 10 sec...");
	u8g2.sendBuffer();
	
	// Calibration process
	// analogRead() takes about 0.1 ms on an AVR.
	// 0.1 ms per sensor * 4 samples per sensor read (default) * 8 sensors
	// * 10 reads per calibrate() call = ~32 ms per calibrate() call.
	// Call calibrate() 400 times to make calibration take about 10 seconds.
	for (uint16_t i = 0; i < 400; i++) {
	qtr.calibrate();
	
	// Update progress on OLED every 20 iterations
	if (i % 20 == 0) {
		u8g2.clearBuffer();
		u8g2.setFont(u8g2_font_6x10_tf);
		u8g2.drawStr(0, 10, "QTR Calibration");
		u8g2.setCursor(0, 20);
		u8g2.print("Progress: ");
		u8g2.print(i * 100 / 400);
		u8g2.print("%");
		
		// Draw progress bar
		u8g2.drawFrame(0, 23, 128, 6);
		u8g2.drawBox(0, 23, map(i, 0, 400, 0, 128), 6);
		
		u8g2.sendBuffer();
	}
	}
	
	digitalWrite(LED_BUILTIN, LOW); // Turn off Arduino's LED to indicate end of calibration
	
	// Display calibration complete message
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_6x10_tf);
	u8g2.drawStr(0, 10, "Calibration Done!");
	u8g2.drawStr(0, 20, "Starting readings...");
	u8g2.sendBuffer();
	
	// Print calibration values to Serial Monitor
	Serial.println("Calibration minimum values:");
	for (uint8_t i = 0; i < NUM_SENSORS; i++) {
	Serial.print(qtr.calibrationOn.minimum[i]);
	Serial.print(' ');
	}
	Serial.println();
	
	Serial.println("Calibration maximum values:");
	for (uint8_t i = 0; i < NUM_SENSORS; i++) {
	Serial.print(qtr.calibrationOn.maximum[i]);
	Serial.print(' ');
	}
	Serial.println();
	Serial.println();
	
	delay(1000);
}