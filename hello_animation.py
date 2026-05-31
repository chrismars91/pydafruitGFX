import pydafruit_gfx as gfx
import math, time

disp = gfx.GFXDisplay(296, 128, scale=4)
disp.begin("Animation Test")

WHITE = 0xFFFF
BLACK = 0x0000
RED   = gfx.color565(220, 0, 0)

t = 0.0
while disp.handle_events():
    disp.fill_screen(WHITE)
    
    # moving circle :)
    x = int(148 + 100 * math.sin(t))
    y = int(64  +  40 * math.cos(t * 1.3))
    disp.fill_circle(x, y, 12, RED)
    disp.draw_circle(x, y, 12, BLACK)
    
    disp.flush()
    t += 0.05
    time.sleep(0.016)  # ~60fps cap