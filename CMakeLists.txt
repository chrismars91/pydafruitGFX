# ---- written with claude

cmake_minimum_required(VERSION 3.18)
project(pydafruit_gfx CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# pybind11 – picked up from the Python environment by scikit-build-core
# ---------------------------------------------------------------------------
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

# ---------------------------------------------------------------------------
# SDL2
# ---------------------------------------------------------------------------
if(APPLE)
    execute_process(
        COMMAND brew --prefix sdl2
        OUTPUT_VARIABLE HOMEBREW_SDL2_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(HOMEBREW_SDL2_PREFIX)
        list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_SDL2_PREFIX}")
        set(SDL2_INCLUDE_DIRS "${HOMEBREW_SDL2_PREFIX}/include/SDL2")
        set(SDL2_LIBRARIES    "${HOMEBREW_SDL2_PREFIX}/lib/libSDL2.dylib")
    endif()
endif()

find_package(SDL2 QUIET)

if(NOT TARGET SDL2::SDL2)
    if(NOT SDL2_INCLUDE_DIRS OR NOT SDL2_LIBRARIES)
        message(FATAL_ERROR
            "SDL2 not found.\n"
            "  macOS : brew install sdl2\n"
            "  Linux : apt-get install libsdl2-dev\n"
            "  Win   : vcpkg install sdl2")
    endif()
    add_library(SDL2::SDL2 INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES      "${SDL2_LIBRARIES}"
    )
endif()

# ---------------------------------------------------------------------------
# Adafruit-GFX-Library  (expected at <source_root>/Adafruit-GFX-Library)
# ---------------------------------------------------------------------------
set(ADAFRUIT_GFX_DIR "${CMAKE_SOURCE_DIR}/Adafruit-GFX-Library"
    CACHE PATH "Path to Adafruit-GFX-Library source")

if(NOT EXISTS "${ADAFRUIT_GFX_DIR}/Adafruit_GFX.h")
    message(FATAL_ERROR
        "Adafruit_GFX.h not found in '${ADAFRUIT_GFX_DIR}'.\n"
        "Run:  git clone https://github.com/adafruit/Adafruit-GFX-Library.git")
endif()

# ---------------------------------------------------------------------------
# Arduino compatibility shims  (written into the build tree)
# ---------------------------------------------------------------------------
set(COMPAT_DIR "${CMAKE_BINARY_DIR}/arduino_compat")
file(MAKE_DIRECTORY "${COMPAT_DIR}/avr")

file(WRITE "${COMPAT_DIR}/Print.h" [[
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
class Print {
public:
    virtual ~Print() = default;
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t* buf, size_t size) {
        size_t n = 0; while (size--) n += write(*buf++); return n;
    }
    size_t write(const char* str) {
        if (!str) return 0;
        return write(reinterpret_cast<const uint8_t*>(str), strlen(str));
    }
    size_t print(const char* s)            { return write(s); }
    size_t print(char c)                   { return write(static_cast<uint8_t>(c)); }
    size_t print(int n,           int = 10){ char b[32]; snprintf(b,sizeof(b),"%d",n);  return write(b); }
    size_t print(unsigned int n,  int = 10){ char b[32]; snprintf(b,sizeof(b),"%u",n);  return write(b); }
    size_t print(long n,          int = 10){ char b[32]; snprintf(b,sizeof(b),"%ld",n); return write(b); }
    size_t print(unsigned long n, int = 10){ char b[32]; snprintf(b,sizeof(b),"%lu",n); return write(b); }
    size_t print(double n, int digits = 2) {
        char fmt[16], buf[64];
        snprintf(fmt,sizeof(fmt),"%%.%df",digits);
        snprintf(buf,sizeof(buf),fmt,n);
        return write(buf);
    }
    size_t println(const char* s)           { size_t n=print(s);   n+=write('\n'); return n; }
    size_t println(char c)                  { size_t n=print(c);   n+=write('\n'); return n; }
    size_t println(int v,          int b=10){ size_t n=print(v,b); n+=write('\n'); return n; }
    size_t println(unsigned int v, int b=10){ size_t n=print(v,b); n+=write('\n'); return n; }
    size_t println(long v,         int b=10){ size_t n=print(v,b); n+=write('\n'); return n; }
    size_t println(unsigned long v,int b=10){ size_t n=print(v,b); n+=write('\n'); return n; }
    size_t println(double v, int d = 2)     { size_t n=print(v,d); n+=write('\n'); return n; }
    size_t println()                        { return write('\n'); }
};
]])

file(WRITE "${COMPAT_DIR}/Arduino.h" [[
#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cmath>
using std::sin; using std::cos; using std::sqrt; using std::atan2; using std::fabs;
using std::min; using std::max; using std::abs;
typedef bool    boolean;
typedef uint8_t byte;
#define PROGMEM
#define pgm_read_byte(addr)   (*(const uint8_t  *)(addr))
#define pgm_read_word(addr)   (*(const uint16_t *)(addr))
#define pgm_read_dword(addr)  (*(const uint32_t *)(addr))
#define pgm_read_ptr(addr)    (*(const void * const *)(addr))
#define memcpy_P  memcpy
#define strlen_P  strlen
#define F(s)      (s)
typedef const char* __FlashStringHelper;
template<typename T> inline void swap(T& a, T& b) { T t=a; a=b; b=t; }
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define radians(deg) ((deg)*M_PI/180.0)
#define degrees(rad) ((rad)*180.0/M_PI)
#define sq(x)        ((x)*(x))
#define constrain(x,lo,hi) ((x)<(lo)?(lo):((x)>(hi)?(hi):(x)))
#include <string>
class String : public std::string {
public:
    String() = default;
    String(const char* s) : std::string(s ? s : "") {}
    String(const std::string& s) : std::string(s) {}
    size_t      length()         const { return std::string::length(); }
    const char* c_str()          const { return std::string::c_str(); }
    char        charAt(size_t i) const { return (*this)[i]; }
};
#ifdef _MSC_VER
#include <stdlib.h>
#define __builtin_bswap16(x) _byteswap_ushort(x)
#define __builtin_bswap32(x) _byteswap_ulong(x)
#define __builtin_bswap64(x) _byteswap_uint64(x)
#endif
#include "Print.h"
]])

file(WRITE "${COMPAT_DIR}/avr/pgmspace.h" [[
#pragma once
#include "../Arduino.h"
]])

foreach(stub IN ITEMS Adafruit_I2CDevice.h Adafruit_SPIDevice.h Adafruit_BusIO_Register.h)
    file(WRITE "${COMPAT_DIR}/${stub}" [[
#pragma once
// Desktop stub
]])
endforeach()

# ---------------------------------------------------------------------------
# adafruit_gfx static library
# ---------------------------------------------------------------------------
add_library(adafruit_gfx STATIC
    "${ADAFRUIT_GFX_DIR}/Adafruit_GFX.cpp"
)
target_include_directories(adafruit_gfx PUBLIC
    "${ADAFRUIT_GFX_DIR}"
    "${COMPAT_DIR}"
)
target_compile_definitions(adafruit_gfx PUBLIC ARDUINO=100)
target_compile_options(adafruit_gfx PRIVATE -w)
set_target_properties(adafruit_gfx PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ---------------------------------------------------------------------------
# pybind11 extension module
# ---------------------------------------------------------------------------
pybind11_add_module(pydafruit_gfx
    pydafruit_gfx.cpp
    GFXDisplay.cpp
)

target_include_directories(pydafruit_gfx PRIVATE
    "${CMAKE_SOURCE_DIR}"
    "${ADAFRUIT_GFX_DIR}"
    "${COMPAT_DIR}"
    "${SDL2_INCLUDE_DIRS}"
)

target_link_libraries(pydafruit_gfx PRIVATE
    adafruit_gfx
    SDL2::SDL2
)

target_compile_options(pydafruit_gfx PRIVATE
    $<IF:$<CXX_COMPILER_ID:MSVC>,/W3,-Wall -Wextra -Wno-unused-parameter>
)

# scikit-build-core places the .so in the right wheel location automatically.
install(TARGETS pydafruit_gfx DESTINATION pydafruit_gfx)
