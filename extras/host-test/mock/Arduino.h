// Minimal Arduino API mock for host-side tests (not used on real boards)
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define HEX 16
#define DEC 10
#define BIN 2

extern uint32_t g_millis;
inline uint32_t millis() { return g_millis; }
inline void delay(uint32_t ms) { g_millis += ms; }
inline void yield() {}

struct MockSerial {
  void begin(unsigned long) {}
  void print(const char *s) { fputs(s, stdout); }
  void print(char c) { putchar(c); }
  void print(float v, int d = 2) { printf("%.*f", d, v); }
  void print(double v, int d = 2) { printf("%.*f", d, v); }
  void print(int v, int base = DEC) { base == HEX ? printf("%X", v) : printf("%d", v); }
  void print(unsigned v, int base = DEC) { base == HEX ? printf("%X", v) : printf("%u", v); }
  void print(long v) { printf("%ld", v); }
  void print(unsigned long v) { printf("%lu", v); }
  template <typename T> void println(T v) { print(v); putchar('\n'); }
  template <typename T> void println(T v, int f) { print(v, f); putchar('\n'); }
  void println() { putchar('\n'); }
};
extern MockSerial Serial;
