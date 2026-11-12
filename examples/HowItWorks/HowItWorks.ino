/*
 * KAPOMKA-DEVICE : HowItWorks  (ตัวอย่างสอนหลักการ สำหรับนักศึกษา)
 *
 * แม่แบบ: เขียนตัวอย่างที่คุยกับอุปกรณ์ "เอง" ทีละขั้น โดยไม่เรียก dev.read()
 * ให้นักศึกษาเห็นว่าไลบรารีทำอะไรอยู่ข้างใน แต่ละขั้นอธิบายเป็นภาษาไทยในคอมเมนต์:
 *
 *   ขั้น 1  ส่งคำสั่ง          (บอกไบต์คำสั่งและความหมายจากดาต้าชีต)
 *   ขั้น 2  รอผล              (ทำไมต้องรอ รอกี่ ms ดูจากบิตไหน)
 *   ขั้น 3  อ่านข้อมูลดิบ       (พิมพ์เป็นเลขฐาน 16 / ฐาน 2 ให้เห็นบิต)
 *   ขั้น 4  แปลงเป็นหน่วยจริง   (สูตรจากดาต้าชีต + เทียบกับฟังก์ชันของไลบรารี)
 *
 * ดูตัวอย่างที่ทำเสร็จแล้วได้ที่ KAPOMKA-AHT10/examples/HowItWorks
 */
#include <Wire.h>
#include <KAPOMKA_DEVICE.h>

const uint8_t ADDR = KAPOMKA_DEVICE::DEFAULT_ADDRESS;

#if defined(CONFIG_IDF_TARGET_ESP32S3)
const int PIN_SDA = 8, PIN_SCL = 9;
#else
const int PIN_SDA = 21, PIN_SCL = 22;
#endif

void setup() {
  Serial.begin(115200);
#if defined(ARDUINO_ARCH_ESP32)
  Wire.begin(PIN_SDA, PIN_SCL);
#else
  Wire.begin();
#endif
}

void loop() {
  // ---------- ขั้น 1 : ส่งคำสั่ง ----------
  Wire.beginTransmission(ADDR);
  Wire.write(0x00);           // TODO: คำสั่งจริงจากดาต้าชีต
  Wire.endTransmission();

  // ---------- ขั้น 2 : รอผล ----------
  delay(10);                  // TODO: เวลาตามดาต้าชีต (ในตัวอย่างใช้ delay ได้ แต่ในไลบรารีห้าม delay ยาว)

  // ---------- ขั้น 3 : อ่านข้อมูลดิบ ----------
  uint8_t b[2] = {0, 0};
  Wire.requestFrom(ADDR, (uint8_t)2);
  for (int i = 0; i < 2; i++) b[i] = Wire.read();
  uint32_t raw = ((uint32_t)b[0] << 8) | b[1];   // ไบต์บนเลื่อนซ้าย 8 บิต แล้วต่อด้วยไบต์ล่าง

  // ---------- ขั้น 4 : แปลงเป็นหน่วยจริง ----------
  Serial.print("raw = ");
  Serial.print((unsigned long)raw);
  Serial.print("  ->  ");
  Serial.println(KAPOMKA_DEVICE::rawToValue(raw), 2);

  delay(2000);
}
