#include <U8g2lib.h>
// #include <SoftWire.h>

#define SDA_PIN 12
#define SCL_PIN 11


U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL_PIN, /* data=*/ SDA_PIN, /* reset=*/ U8X8_PIN_NONE);

void setup() {
    u8g2.begin(); u8g2.clearBuffer(); u8g2.setFont(u8g2_font_6x12_tr); u8g2.drawStr(0, 16, "Display Initialization Complete."); u8g2.sendBuffer();

    delay(2000);
}

void loop() {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 16, "Dhanno Gadha Hai.");
    u8g2.sendBuffer();

    delay(2000);

    u8g2.clearBuffer();
    u8g2.drawStr(0, 16, "LOL");
    u8g2.sendBuffer();

    delay(2000);
}