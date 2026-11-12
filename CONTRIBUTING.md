# การมีส่วนร่วม / Contributing

ยินดีรับ pull request จากนักศึกษาและทุกคน ไม่ต้องเก่งก็ส่งได้ ขอแค่ทำตามขั้นตอนนี้

## ขั้นตอนส่ง pull request (สำหรับมือใหม่)

1. กด **Fork** มุมขวาบนของหน้า repo
2. Clone fork ของคุณ แล้วสร้าง branch ใหม่
   ```sh
   git clone https://github.com/<ชื่อคุณ>/KAPOMKA-DEVICE.git
   cd KAPOMKA-DEVICE
   git checkout -b fix-timeout
   ```
3. แก้โค้ด แล้วทดสอบบนเครื่อง (ต้องมี g++)
   ```sh
   sh extras/host-test/run.sh
   ```
   ถ้ามีบอร์ด ESP32 ให้อัปโหลดตัวอย่าง `Basic` ทดสอบกับเซนเซอร์จริงด้วย
4. เพิ่มบรรทัดใน `CHANGELOG.md` หัวข้อ `[Unreleased]`
5. Commit และ push แล้วกด **Compare & pull request** บน GitHub
6. รอ CI (GitHub Actions) ขึ้นเครื่องหมายถูกสีเขียว ผู้ดูแลจะรีวิวและตอบกลับ

## มาตรฐานโค้ด

- ชื่อ repo `KAPOMKA-<อุปกรณ์>` ชื่อไฟล์/คลาส `KAPOMKA_<อุปกรณ์>` (C++ ใช้ขีดกลางไม่ได้)
- API แบบเดียวกันทุกไลบรารี: `begin()` คืน `bool`, `read()` คืน `bool`, getter มีหน่วยในชื่อ (`getTemperatureC()`), `lastError()`
- รับ `TwoWire&` / `Stream&` จากผู้ใช้ ห้ามผูกขาตายตัวในไลบรารี
- ห้าม `delay()` ยาวในไลบรารี ถ้าต้องรอให้ทำแบบ polling หรือมี API แบบไม่บล็อก
- เขียนจากดาต้าชีต ถ้านำโค้ดจากไลบรารีอื่นมาต้องตรวจไลเซนส์และใส่เครดิต
- **ห้ามใส่รหัส WiFi / MQTT / token หรือข้อมูลลับใด ๆ ลงใน repo**
- คอมเมนต์ในตัวอย่างเขียนไทยได้ คอมเมนต์ในไลบรารีใช้อังกฤษ (ไทยกำกับได้)
- รูปแบบโค้ด: เว้น 2 ช่อง, ปีกกาเปิดบรรทัดเดียวกัน

## รายงานปัญหา

เปิด [Issue](../../issues/new/choose) แล้วเลือกแบบฟอร์ม กรอกบอร์ด การต่อสาย และข้อความจาก Serial Monitor

---

**English:** Fork → branch → change → run `sh extras/host-test/run.sh` → update `CHANGELOG.md` → open a pull request. Keep the common KAPOMKA API (`begin()`/`read()` return `bool`, unit-named getters, `lastError()`), take the bus as a parameter, no long `delay()`, no secrets, and respect other projects' licenses.
