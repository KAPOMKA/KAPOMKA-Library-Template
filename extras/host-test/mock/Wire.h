// TwoWire mock with one simulated device (host tests only).
// Edit FakeDevice to behave like your chip (registers, busy flag, CRC...).
#pragma once
#include "Arduino.h"

struct FakeDevice {
  bool present = true;
  uint8_t addr = 0x00;
  uint8_t reply[16] = {0};   // bytes returned by requestFrom()
  uint8_t lastCmd = 0;
};
extern FakeDevice g_dev;

class TwoWire {
public:
  void begin() {}
  void begin(int, int) {}
  void beginTransmission(uint8_t a) { _a = a; _n = 0; }
  size_t write(uint8_t b) { if (_n < sizeof(_tx)) _tx[_n++] = b; return 1; }
  uint8_t endTransmission() {
    if (!g_dev.present || _a != g_dev.addr) return 2;
    if (_n) g_dev.lastCmd = _tx[0];
    return 0;
  }
  uint8_t requestFrom(uint8_t a, uint8_t n) {
    _rn = _ri = 0;
    if (!g_dev.present || a != g_dev.addr) return 0;
    _rn = n > 16 ? 16 : n;
    memcpy(_rx, g_dev.reply, _rn);
    return _rn;
  }
  uint8_t requestFrom(int a, int n) { return requestFrom((uint8_t)a, (uint8_t)n); }
  int available() { return _rn - _ri; }
  int read() { return _ri < _rn ? _rx[_ri++] : -1; }
private:
  uint8_t _a = 0, _tx[16], _n = 0, _rx[16], _rn = 0, _ri = 0;
};
extern TwoWire Wire;
