#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define IR_PIN 27
#define BUZZER_PIN 26

int lastBuzzTotal = 0;

Adafruit_TCS34725 tcs(
  TCS34725_INTEGRATIONTIME_50MS,
  TCS34725_GAIN_4X
);

int productA = 0;
int productB = 0;
int productC = 0;

bool waitingForColor = false;

void setup() {
  Serial.begin(115200);

  pinMode(IR_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  if (!tcs.begin()) {
    Serial.println("Sensor Not Found");
    while (1);
  }

  Serial.println("System Ready");
}

void loop() {

  bool irState = digitalRead(IR_PIN);

  // ขั้นที่ 1 : IR เจอวัตถุก่อน
  if (irState == LOW && !waitingForColor) {

    Serial.println("Object Detected");

    waitingForColor = true;

    // รอให้นำวัตถุมาอยู่ตรงหน้า TCS34725
    delay(1500);

    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);

    Serial.print("R:");
    Serial.print(r);
    Serial.print(" G:");
    Serial.print(g);
    Serial.print(" B:");
    Serial.print(b);
    Serial.print(" C:");
    Serial.println(c);

    String color = "";

    if (r > g * 1.2 && r > b * 1.2) {
      color = "RED";
      productA++;
    }
    else if (g > r * 1.2 && g > b * 1.2) {
      color = "GREEN";
      productC++;
    }
    else if (b > r * 1.05 && b > g * 1.05) {
      color = "BLUE";
      productB++;
    }

    int total = productA + productB + productC;

    if (total > 0 && total % 10 == 0 && total != lastBuzzTotal) {

    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);

    lastBuzzTotal = total;
    }

    Serial.print("Detected Color: ");
    Serial.println(color);

    Serial.println("==========");
    Serial.print("A (RED) = ");
    Serial.println(productA);

    Serial.print("B (BLUE) = ");
    Serial.println(productB);

    Serial.print("C (GREEN) = ");
    Serial.println(productC);

    Serial.print("TOTAL = ");
    Serial.println(total);

    Serial.println("==========");
  }

  // ขั้นที่ 2 : รอจนวัตถุออกจาก IR ก่อน
  if (irState == HIGH) {
    waitingForColor = false;
  }

  delay(50);
}
