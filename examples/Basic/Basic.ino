/*
 * KAPOMKA-DEVICE : Basic
 * อ่านค่าจาก DEVICE ทุก 2 วินาที แล้วพิมพ์ออก Serial Monitor (115200)
 *
 * ESP32 -> SDA 21, SCL 22 · ESP32-S3 -> SDA 8, SCL 9
 */
#include <Wire.h>
#include <KAPOMKA_DEVICE.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
const int PIN_SDA = 8, PIN_SCL = 9;
#else
const int PIN_SDA = 21, PIN_SCL = 22;
#endif

KAPOMKA_DEVICE dev;

void setup() {
  Serial.begin(115200);
#if defined(ARDUINO_ARCH_ESP32)
  Wire.begin(PIN_SDA, PIN_SCL);
#else
  Wire.begin();
#endif
  if (!dev.begin(Wire)) {
    Serial.print("begin failed: ");
    Serial.println(dev.errorString());
    while (true) delay(1000);
  }
  Serial.println("DEVICE ready");
}

void loop() {
  if (dev.read()) {
    Serial.print("Value: ");
    Serial.println(dev.getValueUnit(), 2);
  } else {
    Serial.print("Read error: ");
    Serial.println(dev.errorString());
  }
  delay(2000);
}
