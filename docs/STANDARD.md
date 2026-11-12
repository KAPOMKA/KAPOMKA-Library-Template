# มาตรฐานไลบรารี KAPOMKA

ใช้กับทุก repo `KAPOMKA-<อุปกรณ์>` · ไลบรารีต้นแบบที่ทำตามมาตรฐานครบ: [KAPOMKA-AHT10](https://github.com/KAPOMKA/KAPOMKA-AHT10)

## 1. ชื่อ

| สิ่ง | รูปแบบ | ตัวอย่าง |
|---|---|---|
| repo / `name` ใน library.properties | `KAPOMKA-<DEVICE>` | `KAPOMKA-AHT10` |
| ไฟล์ header / คลาส | `KAPOMKA_<DEVICE>` | `KAPOMKA_AHT10.h`, `class KAPOMKA_AHT10` |
| มาโครเวอร์ชัน | `KAPOMKA_<DEVICE>_VERSION` | `"0.1.0"` |

C++ ใช้ขีดกลางในชื่อไม่ได้ จึงใช้ขีดล่างในโค้ด

## 2. โครงสร้าง (Arduino Library Specification 1.5 + PlatformIO)

```
KAPOMKA-<DEVICE>/
├── src/KAPOMKA_<DEVICE>.h / .cpp
├── examples/Basic/            อ่านค่าออก Serial
├── examples/HowItWorks/       สอนหลักการ คอมเมนต์ภาษาไทย
├── extras/host-test/          unit test บนคอมพิวเตอร์ด้วยอุปกรณ์จำลอง (Arduino IDE ไม่สนโฟลเดอร์ extras)
├── .github/workflows/ci.yml   host test + arduino-lint + คอมไพล์ ESP32 / ESP32-S3
├── .github/ISSUE_TEMPLATE/    แบบฟอร์มแจ้งปัญหา / ขอฟีเจอร์
├── .github/pull_request_template.md
├── library.properties  library.json  keywords.txt
├── README.md (ไทย + English)  LICENSE (MIT)  CHANGELOG.md  CONTRIBUTING.md
```

## 3. API ร่วม

```cpp
bool begin(TwoWire &wire = Wire, uint8_t address = DEFAULT_ADDRESS);  // อุปกรณ์ UART/RS485: begin(Stream &port, ...)
bool read();                 // อ่านครบหนึ่งรอบ true = ได้ค่าใหม่
bool startMeasurement();     // ถ้าอุปกรณ์ต้องใช้เวลาแปลงค่า
bool update();               // เรียกบ่อย ๆ คืน true ครั้งเดียวเมื่อค่าใหม่พร้อม
bool isConnected();
float getTemperatureC() const;   // getter มีหน่วยในชื่อเสมอ: C, F, RH, Pa, hPa, V, A, W, Wh, mm, ...
Error lastError() const;         // enum class Error : None, NotStarted, NotFound, Busy, Timeout, BusError, Crc, ...ของเฉพาะอุปกรณ์
const char *errorString() const;
static float rawTo...(uint32_t raw);  // สูตรจากดาต้าชีต เปิด public static เพื่อสอนและทดสอบ
```

กติกา:
- ไลบรารี **ไม่เรียก** `Wire.begin()` หรือ `Serial2.begin()` เอง ผู้ใช้กำหนดขาในสเก็ตช์แล้วส่ง bus เข้ามา
- ห้าม `delay()` ยาวในไลบรารี รอผลด้วยการถามสถานะเป็นช่วงสั้น ๆ (≤ 5 ms) และมีแบบไม่บล็อกให้เลือก
- ทุกการรอมี timeout ห้ามวนรอไม่รู้จบ
- ค่าที่ยังไม่เคยอ่านสำเร็จคืน `NAN`
- ไม่ใช้ `String` และ heap ในไลบรารี

## 4. ตัวอย่าง

อย่างน้อย 2 ตัว: `Basic` และ `HowItWorks` (คุยกับอุปกรณ์เองทีละขั้น อธิบายเป็นไทย เหมาะกับนักศึกษา) ทุกตัวต้องคอมไพล์ผ่านบน ESP32 และ ESP32-S3

## 5. README ต้องมี

รูปต่อสาย ESP32 · ตารางขา · ติดตั้งผ่าน PlatformIO / Arduino IDE · ตัวอย่างโค้ด · ตาราง API · หลักการจากดาต้าชีต · **ตารางผลทดสอบกับบอร์ดจริง** · อ้างอิงดาต้าชีต · ส่วนภาษาอังกฤษ

## 6. เวอร์ชันและ release

- SemVer เริ่ม `0.1.0` · ติด tag `vX.Y.Z` ทุกรุ่น · เขียน CHANGELOG ทุกรุ่น
- เปลี่ยนเลขใน `library.properties`, `library.json`, มาโคร `_VERSION` และ README ให้ตรงกัน
- ก่อนออก `1.0.0` ต้องทดสอบกับบอร์ดจริงและบันทึกผลใน README

## 7. ข้อห้าม

- ห้ามคัดลอกโค้ดจากไลบรารีอื่นโดยไม่ตรวจไลเซนส์ เขียนจากดาต้าชีตเป็นหลัก ถ้าดัดแปลงต้องใส่เครดิต
- ห้ามใส่รหัส WiFi / MQTT / token หรือโค้ดของสินค้าที่ขาย (เช่น CFG_SECRET, ระบบสิทธิ์ SF_PREMIUM) ลง repo สาธารณะ
- ต้องถามเจ้าของก่อน: สร้าง repo สาธารณะ, push ครั้งแรก, ออก release
