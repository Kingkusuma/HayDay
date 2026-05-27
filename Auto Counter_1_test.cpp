#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_TCS34725.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_TCS34725 tcs =
Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,
TCS34725_GAIN_4X);

int redCount = 0;
int greenCount = 0;
int blueCount = 0;
int totalCount = 0;

bool detected = false;

void setup() {

  Serial.begin(115200);

  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Sensor
  if (!tcs.begin()) {
    Serial.println("Sensor Error");
    while (1);
  }
}

void loop() {

  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);

  // มีวัตถุ
  if (c > 1000 && detected == false) {

    detected = true;

    if (r > g && r > b) {
      redCount++;
      totalCount++;
    }

    else if (g > r && g > b) {
      greenCount++;
      totalCount++;
    }

    else if (b > r && b > g) {
      blueCount++;
      totalCount++;
    }
  }

  // ไม่มีวัตถุ
  if (c < 500) {
    detected = false;
  }

  // OLED Display
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("RED: ");
  display.println(redCount);

  display.setCursor(0,15);
  display.print("GREEN: ");
  display.println(greenCount);

  display.setCursor(0,30);
  display.print("BLUE: ");
  display.println(blueCount);

  display.setCursor(0,45);
  display.print("TOTAL: ");
  display.println(totalCount);

  display.display();

  delay(100);
}