#include <U8g2lib.h>
#include <SoftWire.h>

#define SDA_PIN 11  // Software I2C SDA pin
#define SCL_PIN 12  // Software I2C SCL pin

// Initialize the SoftWire instance with SDA and SCL
SoftWire myWire(SDA_PIN, SCL_PIN);

// Initialize U8g2 with the SoftWire object for a 128x64 OLED
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL_PIN, SDA_PIN, U8X8_PIN_NONE); // U8g2 128x64 OLED with Software I2C

void setup() {
  // Initialize the U8g2 display
  u8g2.begin();
  
  // Clear the display
  u8g2.clearBuffer();
  
  // Set font and display "Hello World!"
  u8g2.setFont(u8g2_font_ncenB08_tr); // Select a font
  u8g2.drawStr(0, 10, "Hello World!"); // Draw string at (0,10)
  u8g2.sendBuffer(); // Transfer buffer content to the display
}

void loop() {
  // Nothing needed here for static display
}
