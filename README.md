<!-- TEMPLATE-ONLY -->
# KAPOMKA Library Template

แม่แบบสำหรับสร้างไลบรารี `KAPOMKA-<อุปกรณ์>` ทุกตัว ตั้งเป็น **GitHub template repository** แล้วกด *Use this template*
หรือสร้างจากเครื่อง:

```sh
python3 tools/new_library.py BMP280 ../KAPOMKA-BMP280
```

สคริปต์จะเปลี่ยนชื่อไฟล์/คลาส/URL ทั้งหมด ลบโฟลเดอร์ `tools/` และลบส่วนนี้ของ README ออก
มาตรฐานเต็มอยู่ที่ [docs/STANDARD.md](docs/STANDARD.md) · ตัวอย่างที่ทำเสร็จแล้ว: [KAPOMKA-AHT10](https://github.com/KAPOMKA/KAPOMKA-AHT10)

### เช็กลิสต์หลังสร้าง repo ใหม่

- [ ] แก้ทุก `TODO` และ `<...>` ใน `src/`, `examples/`, `library.properties`, `library.json`, README
- [ ] ใส่ที่อยู่ I2C / คำสั่ง / สูตรจากดาต้าชีต และเขียน unit test ใน `extras/host-test/test_main.cpp`
- [ ] แก้ `extras/host-test/mock/Wire.h` ให้จำลองอุปกรณ์จริง (หรือเปลี่ยนเป็น mock `Stream` สำหรับ UART/RS485)
- [ ] เพิ่ม getter และคำสำคัญใน `keywords.txt`
- [ ] `sh extras/host-test/run.sh` ผ่าน
- [ ] ทดสอบกับบอร์ดจริง แล้วกรอกตารางผลทดสอบ
- [ ] ถามเจ้าของก่อนสร้าง repo สาธารณะ / push ครั้งแรก / ออก release

---
<!-- /TEMPLATE-ONLY -->
# KAPOMKA-DEVICE

[![CI](https://github.com/KAPOMKA/KAPOMKA-DEVICE/actions/workflows/ci.yml/badge.svg)](https://github.com/KAPOMKA/KAPOMKA-DEVICE/actions/workflows/ci.yml)
![version](https://img.shields.io/badge/version-0.1.0-blue) ![license](https://img.shields.io/badge/license-MIT-green)

ไลบรารี Arduino สำหรับ **DEVICE** (<อธิบายว่าวัดอะไร ต่อแบบไหน>) จาก KAPOMKA เขียนจากดาต้าชีตของผู้ผลิต

*English summary below.*

> **สถานะ:** 0.1.0 ยังไม่ได้ทดสอบกับบอร์ดจริง

## การต่อสาย ESP32

```
   DEVICE                      ESP32 DevKit
  ┌────────────┐               ┌──────────────┐
  │ VCC   ●────┼───────────────┼──● 3V3       │
  │ GND   ●────┼───────────────┼──● GND       │
  │ SCL   ●────┼───────────────┼──● GPIO22    │
  │ SDA   ●────┼───────────────┼──● GPIO21    │
  └────────────┘               └──────────────┘
```

| ขาอุปกรณ์ | ESP32 | ESP32-S3 | หมายเหตุ |
|---|---|---|---|
| VCC | 3V3 | 3V3 | <ช่วงแรงดันจากดาต้าชีต> |
| GND | GND | GND | |
| SCL | GPIO22 | GPIO9 | |
| SDA | GPIO21 | GPIO8 | |

## ติดตั้ง

**PlatformIO**
```ini
lib_deps =
    https://github.com/KAPOMKA/KAPOMKA-DEVICE.git#v0.1.0
```

**Arduino IDE** — ดาวน์โหลด ZIP จาก [Releases](https://github.com/KAPOMKA/KAPOMKA-DEVICE/releases) แล้ว *Sketch → Include Library → Add .ZIP Library…*

## ตัวอย่างโค้ด

```cpp
#include <Wire.h>
#include <KAPOMKA_DEVICE.h>

KAPOMKA_DEVICE dev;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  if (!dev.begin(Wire)) Serial.println(dev.errorString());
}

void loop() {
  if (dev.read()) Serial.println(dev.getValueUnit());
  delay(2000);
}
```

| ตัวอย่าง | สอนอะไร |
|---|---|
| `Basic` | อ่านค่าออก Serial |
| `HowItWorks` | คุยกับอุปกรณ์เองทีละขั้น อธิบายเป็นภาษาไทย |

## API

| ฟังก์ชัน | คืนค่า | คำอธิบาย |
|---|---|---|
| `begin(TwoWire& = Wire, uint8_t addr)` | `bool` | |
| `read()` | `bool` | |
| `startMeasurement()` / `update()` | `bool` | แบบไม่บล็อก |
| `getValueUnit()` | `float` | |
| `lastError()` / `errorString()` | `Error` / `const char*` | |

## หลักการทำงาน (สรุปจากดาต้าชีต)

| ขั้น | คำสั่ง / ข้อมูล | เวลา |
|---|---|---|
| | | |

## ผลทดสอบกับบอร์ดจริง / Hardware test results

| วันที่ | บอร์ด | Core | อุปกรณ์ | ผล |
|---|---|---|---|---|
| — | ESP32 DevKit | — | DEVICE | ยังไม่ได้ทดสอบ |

## อ้างอิง

- <ชื่อดาต้าชีต, ผู้ผลิต, รุ่นเอกสาร>

## ร่วมพัฒนา

ดู [CONTRIBUTING.md](CONTRIBUTING.md)

---

## English

Arduino library for the **DEVICE** (<what it measures>), written from the manufacturer datasheet.
Standard KAPOMKA API: `begin()` / `read()` return `bool`, unit-named getters, `lastError()`, bus passed in by the sketch, non-blocking option.

License: [MIT](LICENSE) © 2026 KAPOMKA
