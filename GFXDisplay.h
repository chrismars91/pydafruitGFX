#pragma once



#include <SDL2/SDL.h>
#include <cstdint>
#include <string>


#include "Adafruit_GFX.h"

class GFXDisplay : public Adafruit_GFX {
public:
    // width + height
    // scale
    GFXDisplay(int16_t width, int16_t height, int scale = 2);
    ~GFXDisplay() override;

    // open the SDL2 window.
    bool begin(const std::string& title = "Adafruit_GFX Desktop");

    // all the exposed Adafruit_GFX stuff calls this.
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    // Flush pixel buffer.
    // call this at the end of each frame to see updates.
    void flush();

    // false when the window has been closed. true window is open
    bool handleEvents();
    bool isOpen() const { return _open; }

    template<typename Fn>
    void run(Fn render_fn) {
        while (handleEvents()) {
            render_fn(*this);
            flush();
        }
    }

    // Convert RGB888 RGB565
    static constexpr uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

    // Convert RGB565 to SDL_Color, this seems to wrok
    static SDL_Color colorFromRGB565(uint16_t c) {
        return {
            static_cast<uint8_t>((c >> 8) & 0xF8),
            static_cast<uint8_t>((c >> 3) & 0xFC),
            static_cast<uint8_t>((c & 0x1F) << 3),
            255
        };
    }

    void saveBMP(const char* filename) const;

private:
    int          _scale;
    SDL_Window*  _window  = nullptr;
    SDL_Renderer* _renderer = nullptr;
    SDL_Texture* _texture = nullptr;

    // raw RGB565 back-buffer
    uint16_t*    _fb = nullptr;
    bool         _open = false;

    void _destroySDL();
};