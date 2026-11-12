/*
 * KAPOMKA_DEVICE - implementation (template skeleton)
 * SPDX-License-Identifier: MIT
 */
#include "KAPOMKA_DEVICE.h"

static const uint32_t CONVERSION_MS = 10;  // TODO: datasheet conversion time

KAPOMKA_DEVICE::KAPOMKA_DEVICE()
    : _wire(nullptr), _addr(DEFAULT_ADDRESS), _started(false), _measuring(false), _startMs(0),
      _timeoutMs(100), _raw(0), _value(NAN), _error(Error::NotStarted) {}

bool KAPOMKA_DEVICE::begin(TwoWire &wire, uint8_t address) {
  _wire = &wire;
  _addr = address;
  _started = false;
  _measuring = false;
  if (!isConnected()) return fail(Error::NotFound);
  // TODO: check chip ID / reset / configure per datasheet
  _started = true;
  _error = Error::None;
  return true;
}

bool KAPOMKA_DEVICE::isConnected() {
  if (!_wire) return false;
  _wire->beginTransmission(_addr);
  return _wire->endTransmission() == 0;
}

bool KAPOMKA_DEVICE::startMeasurement() {
  if (!_started) return fail(Error::NotStarted);
  if (_measuring) return fail(Error::Busy);
  // TODO: send the "start conversion" command
  _measuring = true;
  _startMs = millis();
  _error = Error::None;
  return true;
}

bool KAPOMKA_DEVICE::update() {
  if (!_measuring) return false;
  uint32_t elapsed = millis() - _startMs;
  if (elapsed < CONVERSION_MS) return false;
  if (!readRaw()) {
    if (_error == Error::Busy && elapsed <= _timeoutMs) { _error = Error::None; return false; }
    if (_error == Error::Busy) _error = Error::Timeout;
    _measuring = false;
    return false;
  }
  _measuring = false;
  return true;
}

bool KAPOMKA_DEVICE::read() {
  if (!startMeasurement()) return false;
  while (_measuring) {
    if (update()) return true;
    if (_error != Error::None) return false;
    delay(2);  // short poll only; never one long delay() inside the library
  }
  return _error == Error::None;
}

bool KAPOMKA_DEVICE::readRaw() {
  // TODO: read result registers, check status/CRC, convert
  const uint8_t n = 2;
  if (_wire->requestFrom(_addr, n) != n) return fail(Error::BusError);
  uint8_t hi = _wire->read(), lo = _wire->read();
  _raw = ((uint32_t)hi << 8) | lo;
  _value = rawToValue(_raw);
  _error = Error::None;
  return true;
}

const char *KAPOMKA_DEVICE::errorString() const {
  switch (_error) {
    case Error::None: return "OK";
    case Error::NotStarted: return "not started";
    case Error::NotFound: return "device not found";
    case Error::Busy: return "busy";
    case Error::Timeout: return "timeout";
    case Error::BusError: return "bus error";
    case Error::Crc: return "CRC error";
  }
  return "unknown";
}
