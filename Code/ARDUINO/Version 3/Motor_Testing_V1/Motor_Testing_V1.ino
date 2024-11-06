#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MOTOR1A 5
#define MOTOR1B 6
#define MOTOR2A 3
#define MOTOR2B 11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("Initializing...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
  }

  pinMode(MOTOR1A, OUTPUT);
  pinMode(MOTOR1B, OUTPUT);
  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);

  Serial.println("Initialization Done");

  display.display();
  delay(5000);


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initialization Done");
  display.println("");
  display.println("");
  display.println("Made By Pranav Verma.");
  display.display();
  delay(4000);
}

void loop() {
  digitalWrite(MOTOR1A, HIGH);
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR2A, HIGH);
  digitalWrite(MOTOR2B, LOW);

  Serial.println("Motors Moving Forward");


  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Motors Moving Forward");
  display.println("");
  display.println("");
  display.println("Made By Pranav Verma.");
  display.display();

  delay(2000);

  // Stop motors
  digitalWrite(MOTOR1A, LOW);
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR2A, LOW);
  digitalWrite(MOTOR2B, LOW);

  Serial.println("Motors Stopped");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Motors Stopped.");
  display.println("");
  display.println("");
  display.println("Made By Pranav Verma.");
  display.display();

  delay(1000);

  // Move motors backward
  digitalWrite(MOTOR1A, LOW);
  digitalWrite(MOTOR1B, HIGH);
  digitalWrite(MOTOR2A, LOW);
  digitalWrite(MOTOR2B, HIGH);

  Serial.println("Motors Moving Back");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Motors Moving");
  display.println("Backwards");
  display.println("");
  display.println("Made By Pranav Verma.");
  display.display();

  delay(2000);

  // Stop motors
  digitalWrite(MOTOR1A, LOW);
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR2A, LOW);
  digitalWrite(MOTOR2B, LOW);

  Serial.println("Motors Stopped");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Motors Stopped.");
  display.println("");
  display.println("");
  display.println("Made By Pranav Verma.");
  display.display();

  delay(1000);
}
