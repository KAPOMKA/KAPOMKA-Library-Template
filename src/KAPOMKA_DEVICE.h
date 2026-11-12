/*
 * KAPOMKA_DEVICE - Library for the <MANUFACTURER> <DEVICE> (<BUS>)
 * ไลบรารีสำหรับ <DEVICE> (<อธิบายสั้น ๆ เป็นไทย>)
 *
 * Written from the manufacturer datasheet. If any code is adapted from another
 * project, credit it here with its license.
 * https://github.com/KAPOMKA/KAPOMKA-DEVICE
 * SPDX-License-Identifier: MIT
 *
 * TEMPLATE: replace DEVICE / <...> placeholders. tools/new_library.py does the renaming.
 */
#ifndef KAPOMKA_DEVICE_H
#define KAPOMKA_DEVICE_H

#include <Arduino.h>
#include <Wire.h>   // RS485/UART devices: use Stream& instead of TwoWire&

#define KAPOMKA_DEVICE_VERSION "0.1.0"

class KAPOMKA_DEVICE {
public:
  static const uint8_t DEFAULT_ADDRESS = 0x00;  // TODO: datasheet I2C address

  // Standard KAPOMKA error codes. Keep the first ones identical across libraries,
  // add device-specific codes at the end.
  enum class Error : uint8_t {
    None = 0,    // no error
    NotStarted,  // begin() not called or failed
    NotFound,    // device does not answer
    Busy,        // operation already running
    Timeout,     // device did not finish in time
    BusError,    // write/read on the bus failed
    Crc          // checksum mismatch
    // device-specific codes here
  };

  KAPOMKA_DEVICE();

  // ---- standard API (same shape in every KAPOMKA library) ----
  bool begin(TwoWire &wire = Wire, uint8_t address = DEFAULT_ADDRESS);  // sketch calls Wire.begin(SDA,SCL) first
  bool read();                 // one complete reading, true = new values available
  bool startMeasurement();     // non-blocking start (if the device needs conversion time)
  bool update();               // call often; true once when new data is ready
  bool isMeasuring() const { return _measuring; }
  bool isConnected();

  // Getters carry the unit in the name, e.g. getTemperatureC(), getPressurePa(), getVoltageV()
  float getValueUnit() const { return _value; }  // TODO: rename + add more getters

  Error lastError() const { return _error; }
  const char *errorString() const;
  void setTimeout(uint16_t ms) { _timeoutMs = ms; }

  // Conversion formulas from the datasheet: keep them public + static so they can be taught and unit-tested
  static float rawToValue(uint32_t raw) { return (float)raw; }  // TODO: datasheet formula

private:
  bool fail(Error e) { _error = e; return false; }
  bool readRaw();

  TwoWire *_wire;
  uint8_t _addr;
  bool _started;
  bool _measuring;
  uint32_t _startMs;
  uint16_t _timeoutMs;
  uint32_t _raw;
  float _value;
  Error _error;
};

#endif
