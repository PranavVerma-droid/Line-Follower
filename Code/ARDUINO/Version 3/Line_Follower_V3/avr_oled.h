#ifndef AVR_OLED_H_
#define AVR_OLED_H_

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class AVR_OLED {
public:
    AVR_OLED();
    void init();
    void clearDisplay();
    void displayText(const char* text, uint8_t x, uint8_t y);
    void display();

private:
    Adafruit_SSD1306 oledDisplay;
};

AVR_OLED::AVR_OLED() : oledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void AVR_OLED::init() {
    oledDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    oledDisplay.display();
    delay(5000);

    oledDisplay.clearDisplay();
    oledDisplay.setTextSize(1);
    oledDisplay.setTextColor(SSD1306_WHITE);
    oledDisplay.setCursor(0, 0);
    oledDisplay.println("Initialization Done");
    oledDisplay.println("");
    oledDisplay.println("");
    oledDisplay.println("Made By Pranav Verma.");
    oledDisplay.display();
    delay(4000);
}

void AVR_OLED::clearDisplay() {
    oledDisplay.clearDisplay();
}

void AVR_OLED::displayText(const char* text, uint8_t x, uint8_t y) {
    oledDisplay.setCursor(x, y);
    oledDisplay.println(text);
}

void AVR_OLED::display() {
    oledDisplay.display();
}

#endif // AVR_OLED_H_
