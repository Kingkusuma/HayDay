#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs =
  Adafruit_TCS34725(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X
  );

int productA = 0; // แดง
int productB = 0; // น้ำเงิน
int productC = 0; // เขียว

bool objectDetected = false;

void setup() {
  Serial.begin(115200);

  if (tcs.begin()) {
    Serial.println("TCS34725 Ready");
  } else {
    Serial.println("Sensor Not Found");
    while (1);
  }
}

void loop() {

  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  // แสดงค่า RGB เพื่อดูการทำงาน
  Serial.print("R:");
  Serial.print(r);
  Serial.print(" G:");
  Serial.print(g);
  Serial.print(" B:");
  Serial.print(b);
  Serial.print(" C:");
  Serial.println(c);

  // ไม่มีวัตถุอยู่หน้าเซ็นเซอร์
  if (c < 500) {
    objectDetected = false;
    delay(100);
    return;
  }

  String color = "";

  // ตรวจจับสี
  if (r > g * 1.2 && r > b * 1.2) {
    color = "RED";
  }
  else if (g > r * 1.2 && g > b * 1.2) {
    color = "GREEN";
  }
  else if (b > r * 1.2 && b > g * 1.2) {
    color = "BLUE";
  }

  // นับครั้งเดียวต่อวัตถุ 1 ชิ้น
  if (!objectDetected && color != "") {

    objectDetected = true;

    if (color == "RED") {
      productA++;
      Serial.println("สินค้า A (สีแดง)");
    }
    else if (color == "BLUE") {
      productB++;
      Serial.println("สินค้า B (สีน้ำเงิน)");
    }
    else if (color == "GREEN") {
      productC++;
      Serial.println("สินค้า C (สีเขียว)");
    }

    int total = productA + productB + productC;

    Serial.println("====================");
    Serial.print("สินค้า A = ");
    Serial.println(productA);

    Serial.print("สินค้า B = ");
    Serial.println(productB);

    Serial.print("สินค้า C = ");
    Serial.println(productC);

    Serial.print("รวมทั้งหมด = ");
    Serial.println(total);
    Serial.println("====================");
  }

  delay(500);
}