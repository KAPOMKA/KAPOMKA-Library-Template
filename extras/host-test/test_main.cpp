// Host-side unit tests (template). Add a test for every datasheet formula and every Error code.
#include <Arduino.h>
#include <Wire.h>
#include "KAPOMKA_DEVICE.h"

uint32_t g_millis = 0;
MockSerial Serial;
TwoWire Wire;
FakeDevice g_dev;

static int fails = 0, checks = 0;
#define CHECK(c) do { checks++; if (!(c)) { fails++; printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #c); } } while (0)

static void reset() { g_dev = FakeDevice(); g_millis = 0; }

int main() {
  using E = KAPOMKA_DEVICE::Error;

  { reset(); KAPOMKA_DEVICE d; CHECK(!d.read()); CHECK(d.lastError() == E::NotStarted); CHECK(isnan(d.getValueUnit())); }
  { reset(); g_dev.present = false; KAPOMKA_DEVICE d; CHECK(!d.begin(Wire)); CHECK(d.lastError() == E::NotFound); }
  { reset(); g_dev.reply[0] = 0x12; g_dev.reply[1] = 0x34; KAPOMKA_DEVICE d;
    CHECK(d.begin(Wire)); CHECK(d.read()); CHECK(d.getValueUnit() == (float)0x1234); }
  { reset(); KAPOMKA_DEVICE d; CHECK(d.begin(Wire)); CHECK(d.startMeasurement()); CHECK(!d.startMeasurement());
    CHECK(d.lastError() == E::Busy); CHECK(!d.update()); g_millis += 20; CHECK(d.update()); }
  { reset(); KAPOMKA_DEVICE d; CHECK(d.begin(Wire)); g_dev.present = false; CHECK(!d.read()); CHECK(d.lastError() == E::BusError); }

  printf("%d/%d checks passed\n", checks - fails, checks);
  return fails ? 1 : 0;
}
