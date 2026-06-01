# pydafruit_gfx

a python tool to quickly prototype [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) displays. Uses an SDL2 backend and pybind11. SDL2 does not need to be downloaded, should be built into the wheels

![Hello World Example](hello_world.bmp)

## layout

```
pydafruit_gfx/
├── .github/
│   └── workflows/
│       └── wheels.yml          github wheels CI
├── Adafruit-GFX-Library/       git submodule
├── pydafruit_gfx/
│   └── __init__.py
├── CMakeLists.txt              wheel build
├── GFXDisplay.h
├── GFXDisplay.cpp
├── gfxdisplay_py.cpp           pybind11
├── pyproject.toml
├── hello_world.py
└── README.md
```

## Install

### Option 1 — download, pip install

Pre-built wheels for macOS, Linux, and Windows
[Actions tab](https://github.com/chrismars91/pydafruitGFX/actions/runs/26725389812).

1. Click the latest successful **Build wheels** run.
2. Scroll to **Artifacts** at the bottom and download the zip for your platform:
   - `wheels-macos-latest` — macOS arm64 + x86_64, Python 3.9–3.12
   - `wheels-ubuntu-latest` — Linux x86_64, Python 3.9–3.12
   - `wheels-windows-latest` — Windows x64, Python 3.9–3.12 (SDL2 bundled)
3. Unzip and install:
```bash
pip install pydafruit_gfx-*.whl
```

## How I tested on my Mac (windows and linux should be similar)

1. ```uname -m``` gave `arm64`
2. ```python3 --version``` gave `Python 3.11.5`
3. ```python --version``` gave `Python 3.11.5`
4. ```pip install pydafruit_gfx-0.1.0-cp311-cp311-macosx_15_0_arm64.whl```

I have a arm64 mac with python 3.11.5 so i used 
- Wheel: `pydafruit_gfx-0.1.0-cp311-cp311-macosx_15_0_arm64.whl`
---

### Option 2 — Build from source

```bash
# 1. Clone with the Adafruit submodule
git clone --recurse-submodules https://github.com/chrismars91/PydafruitGFX.git
cd pydafruitGFX

# 2. Install SDL2
#    macOS:  brew install sdl2
#    Linux:  sudo apt install libsdl2-dev
#    Win:    vcpkg install sdl2:x64-windows

# 3. Build + install
pip install scikit-build-core pybind11
pip install -e . --no-build-isolation -Cbuild-dir=_build

# 4. Run the hello-world example
python hello_world.py
```

---
