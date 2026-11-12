#!/usr/bin/env python3
"""
Create a new KAPOMKA library from this template.
สร้างไลบรารีใหม่จากแม่แบบนี้

    python3 tools/new_library.py BMP280 ../KAPOMKA-BMP280

Replaces KAPOMKA-DEVICE / KAPOMKA_DEVICE / DEVICE in file names and contents,
and removes template-only files (tools/, template notes in README).
"""
import pathlib
import re
import shutil
import sys

SKIP_DIRS = {".git", "build", ".pio"}
TEXT_EXT = {".h", ".cpp", ".ino", ".md", ".txt", ".properties", ".json", ".yml", ".yaml", ".sh", ".gitignore", ""}


def main() -> int:
    if len(sys.argv) != 3:
        print(__doc__)
        return 1
    device = sys.argv[1].upper()
    if not re.fullmatch(r"[A-Z0-9_]+", device):
        print("DEVICE must be A-Z, 0-9 or _ (e.g. BMP280, RS485_SOIL7)")
        return 1
    src = pathlib.Path(__file__).resolve().parent.parent
    dst = pathlib.Path(sys.argv[2]).resolve()
    if dst.exists():
        print(f"{dst} already exists")
        return 1

    shutil.copytree(src, dst, ignore=shutil.ignore_patterns(*SKIP_DIRS))
    shutil.rmtree(dst / "tools")

    repl = [
        ("KAPOMKA-DEVICE", f"KAPOMKA-{device.replace('_', '-')}"),
        ("KAPOMKA_DEVICE", f"KAPOMKA_{device}"),
        ("DEVICE", device),
    ]
    for p in sorted(dst.rglob("*"), key=lambda x: -len(x.parts)):
        if p.name == "STANDARD.md":
            continue
        if p.is_file() and (p.suffix in TEXT_EXT or p.name.startswith(".")):
            t = p.read_text(encoding="utf-8")
            for a, b in repl:
                t = t.replace(a, b)
            t = re.sub(r"<!-- TEMPLATE-ONLY -->.*?<!-- /TEMPLATE-ONLY -->\n?", "", t, flags=re.S)
            p.write_text(t, encoding="utf-8")
        new_name = p.name
        for a, b in repl:
            new_name = new_name.replace(a, b)
        if new_name != p.name:
            p.rename(p.with_name(new_name))

    print(f"Created {dst}")
    print("Next: fill in every TODO and <...> placeholder, then run: sh extras/host-test/run.sh")
    return 0


if __name__ == "__main__":
    sys.exit(main())
