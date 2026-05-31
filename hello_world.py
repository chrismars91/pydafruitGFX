"""
hello_world.py
"""

import pydafruit_gfx as pyfx

# ---- display params -------------------------------------------------------
WIDTH, HEIGHT = 296, 128
SCALE = 4          # zoom factor for the SDL2 window

WHITE = 0xFFFF
BLACK = 0x0000
RED   = pyfx.color565(220, 0, 0)
GRAY  = pyfx.color565(120, 120, 120)

# ---- open ------------------------------------------------------------------
disp = pyfx.GFXDisplay(WIDTH, HEIGHT, scale=SCALE)
if not disp.begin("pydafruit_gfx — Hello World"):
    raise RuntimeError("Failed to open SDL2 window")

# ---- draw! -----------------------------------------------------------------
disp.fill_screen(WHITE)

# border
disp.draw_rect(1, 1, WIDTH - 2, HEIGHT - 2, BLACK)

# custom font, added to pydafruit_gfx.cpp and Adafruit-GFX-Library/Fonts
disp.set_font("GeorgiaI12")
disp.set_text_color(BLACK)
# get text bounds adafruit style
x1, y1, w, h = disp.get_text_bounds("Hello!", 0, 0)
disp.set_cursor((WIDTH - w) // 2, (HEIGHT - h) // 4)
disp.print("Hello!")

# default 5x7 font
disp.set_font_default()


TEXT      = "Hello, World!"
CHAR_W    = 6 * 2
CHAR_H    = 8 * 2
text_px_w = len(TEXT) * CHAR_W
cx = (WIDTH  - text_px_w) // 2
cy = (HEIGHT - CHAR_H)    // 2

disp.set_text_size(2)
# disp.set_text_color(BLACK)
disp.set_cursor(cx, cy)
disp.print(TEXT)


SUBTITLE = "Adafruit GFX  |  SDL2  |  pybind11"
sub_px_w = len(SUBTITLE) * 6
disp.set_text_size(1)
disp.set_text_color(GRAY)
disp.set_cursor((WIDTH - sub_px_w) // 2, cy + CHAR_H + 6)
disp.print(SUBTITLE)


disp.draw_fast_hline(20, cy - 6,        WIDTH - 40, GRAY)
disp.draw_fast_hline(20, cy + CHAR_H + 4, WIDTH - 40, GRAY)

disp.fill_circle(WIDTH - 18, 18, 10, RED)
disp.draw_circle(WIDTH - 18, 18, 10, BLACK)

disp.flush()
disp.save_bmp("hello_world.bmp")
print("Saved hello_world.bmp — close the window to quit.")

while disp.is_open():
    disp.handle_events()
