#!/bin/sh
# Host-side tests: unit tests with a simulated sensor + compile every example against the mock.
# ทดสอบบนคอมพิวเตอร์ (ไม่ต้องมีบอร์ด): unit test กับเซนเซอร์จำลอง + คอมไพล์ตัวอย่างทุกตัว
set -e
cd "$(dirname "$0")"
ROOT=../..
CXX=${CXX:-g++}
FLAGS="-std=c++17 -Wall -Wextra -Werror -Imock -I$ROOT/src"
mkdir -p build

$CXX $FLAGS test_main.cpp $ROOT/src/*.cpp -o build/test_main
./build/test_main

for ino in $ROOT/examples/*/*.ino; do
  name=$(basename "$ino" .ino)
  {
    echo '#include <Arduino.h>'
    echo "#line 1 \"$ino\""
    cat "$ino"
    echo
    echo 'uint32_t g_millis; MockSerial Serial; TwoWire Wire; FakeDevice g_dev;'
    echo 'int main(){ setup(); for(int i=0;i<3;i++) loop(); return 0; }'
  } > build/$name.cpp
  $CXX $FLAGS -Wno-unused-parameter build/$name.cpp $ROOT/src/*.cpp -o build/$name
  ./build/$name > build/$name.log
  echo "example $name: compiled + ran OK"
done
