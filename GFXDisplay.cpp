// GFXDisplay.cpp
#include "GFXDisplay.h"

#include <cstring>
#include <cstdio>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Uses SDL
// ---------------------------------------------------------------------------

GFXDisplay::GFXDisplay(int16_t width, int16_t height, int scale)
    : Adafruit_GFX(width, height)
    , _scale(scale < 1 ? 1 : scale)
{
    _fb = new uint16_t[width * height];
    std::memset(_fb, 0, sizeof(uint16_t) * width * height);
}

GFXDisplay::~GFXDisplay() {
    _destroySDL();
    delete[] _fb;
}

void GFXDisplay::_destroySDL() {
    if (_texture)  { SDL_DestroyTexture(_texture);   _texture  = nullptr; }
    if (_renderer) { SDL_DestroyRenderer(_renderer); _renderer = nullptr; }
    if (_window)   { SDL_DestroyWindow(_window);     _window   = nullptr; }
    SDL_Quit();
    _open = false;
}

// ---------------------------------------------------------------------------
// init
// ---------------------------------------------------------------------------

bool GFXDisplay::begin(const std::string& title) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return false;
    }

    const int winW = _width  * _scale;
    const int winH = _height * _scale;

    _window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        winW, winH,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!_window) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    _renderer = SDL_CreateRenderer(
        _window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!_renderer) {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        _destroySDL();
        return false;
    }

    // upload the framebuffger directly.
    _texture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        _width, _height
    );
    if (!_texture) {
        fprintf(stderr, "SDL_CreateTexture error: %s\n", SDL_GetError());
        _destroySDL();
        return false;
    }

    // scale window size.
    SDL_RenderSetLogicalSize(_renderer, _width, _height);

    _open = true;
    return true;
}

// ---------------------------------------------------------------------------
// drawPixel() – epxose the only Adafruit_GFX draw tool ¯\_(ツ)_/¯
// more may be needed
// ---------------------------------------------------------------------------

void GFXDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    // add transforms here.
    if (x < 0 || x >= _width || y < 0 || y >= _height) return;
    _fb[y * _width + x] = color;
}

// ---------------------------------------------------------------------------
// flush()
// ---------------------------------------------------------------------------

void GFXDisplay::flush() {
    if (!_open) return;

    SDL_UpdateTexture(
        _texture,
        nullptr,
        _fb,
        _width * sizeof(uint16_t)
    );

    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
}

// ---------------------------------------------------------------------------
// handleEvents() – SDL event queue
// ---------------------------------------------------------------------------

bool GFXDisplay::handleEvents() {
    if (!_open) return false;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            _destroySDL();
            return false;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    _destroySDL();
                    return false;
                default:
                    break;
            }
        }
    }
    return _open;
}

// SDL_Renderer* GFXDisplay::getRenderer() const {
//     return _renderer;
// }

void GFXDisplay::saveBMP(const char* filename) const {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        _width,
        _height,
        16,
        SDL_PIXELFORMAT_RGB565
    );

    if (!surface) {
        fprintf(stderr, "SDL_CreateRGBSurface error: %s\n", SDL_GetError());
        return;
    }

    // copy framebuffer
    std::memcpy(surface->pixels, _fb, _width * _height * sizeof(uint16_t));

    SDL_SaveBMP(surface, filename);
    SDL_FreeSurface(surface);
}