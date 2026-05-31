// pydafruit_gfx.cpp
// pybind11 bindings for GFXDisplay.

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "GFXDisplay.h"

// Adafruit fonts - add/remove as needed.
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSerif18pt7b.h>
#include <Fonts/FreeSerif24pt7b.h>
#include <Fonts/Org_01.h>
#include <Fonts/TomThumb.h>

// custom font
#include "Fonts/GeorgiaItal12pt7b.h"


namespace py = pybind11;

PYBIND11_MODULE(pydafruit_gfx, m) {
    m.doc() = "Adafruit_GFX desktop simulator — SDL2 backend, pybind11 bindings";

    m.def("color565",
          [](uint8_t r, uint8_t g, uint8_t b) {
              return GFXDisplay::color565(r, g, b);
          },
          py::arg("r"), py::arg("g"), py::arg("b"),
          "Pack 8-bit R,G,B into a 16-bit RGB565 colour word.");

    // -- GFXDisplay class ----------------------------------------------------
    py::class_<GFXDisplay>(m, "GFXDisplay")
        .def(py::init<int16_t, int16_t, int>(),
             py::arg("width"), py::arg("height"), py::arg("scale") = 2)

        // lifecycle
        .def("begin",         &GFXDisplay::begin,
             py::arg("title") = "Adafruit_GFX Desktop")
        .def("flush",         &GFXDisplay::flush)
        .def("handle_events", &GFXDisplay::handleEvents)
        .def("is_open",       &GFXDisplay::isOpen)
        .def("save_bmp",
             [](GFXDisplay& d, const std::string& path){ d.saveBMP(path.c_str()); },
             py::arg("filename"))

        // screen
        .def("fill_screen",
             [](GFXDisplay& d, uint16_t c){ d.fillScreen(c); },
             py::arg("color"))

        // pixels
        .def("draw_pixel",
             [](GFXDisplay& d, int16_t x, int16_t y, uint16_t c){
                 d.drawPixel(x, y, c);
             }, py::arg("x"), py::arg("y"), py::arg("color"))

        // lines
        .def("draw_line",
             [](GFXDisplay& d, int16_t x0,int16_t y0,int16_t x1,int16_t y1,uint16_t c){
                 d.drawLine(x0,y0,x1,y1,c);
             }, py::arg("x0"),py::arg("y0"),py::arg("x1"),py::arg("y1"),py::arg("color"))
        .def("draw_fast_hline",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t w,uint16_t c){
                 d.drawFastHLine(x,y,w,c);
             }, py::arg("x"),py::arg("y"),py::arg("w"),py::arg("color"))
        .def("draw_fast_vline",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t h,uint16_t c){
                 d.drawFastVLine(x,y,h,c);
             }, py::arg("x"),py::arg("y"),py::arg("h"),py::arg("color"))

        // rectangles
        .def("draw_rect",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t w,int16_t h,uint16_t c){
                 d.drawRect(x,y,w,h,c);
             }, py::arg("x"),py::arg("y"),py::arg("w"),py::arg("h"),py::arg("color"))
        .def("fill_rect",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t w,int16_t h,uint16_t c){
                 d.fillRect(x,y,w,h,c);
             }, py::arg("x"),py::arg("y"),py::arg("w"),py::arg("h"),py::arg("color"))
        .def("draw_round_rect",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t w,int16_t h,int16_t r,uint16_t c){
                 d.drawRoundRect(x,y,w,h,r,c);
             }, py::arg("x"),py::arg("y"),py::arg("w"),py::arg("h"),py::arg("r"),py::arg("color"))
        .def("fill_round_rect",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t w,int16_t h,int16_t r,uint16_t c){
                 d.fillRoundRect(x,y,w,h,r,c);
             }, py::arg("x"),py::arg("y"),py::arg("w"),py::arg("h"),py::arg("r"),py::arg("color"))

        // circles
        .def("draw_circle",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t r,uint16_t c){
                 d.drawCircle(x,y,r,c);
             }, py::arg("x"),py::arg("y"),py::arg("r"),py::arg("color"))
        .def("fill_circle",
             [](GFXDisplay& d, int16_t x,int16_t y,int16_t r,uint16_t c){
                 d.fillCircle(x,y,r,c);
             }, py::arg("x"),py::arg("y"),py::arg("r"),py::arg("color"))

        // triangles
        .def("draw_triangle",
             [](GFXDisplay& d,
                int16_t x0,int16_t y0,int16_t x1,int16_t y1,int16_t x2,int16_t y2,
                uint16_t c){ d.drawTriangle(x0,y0,x1,y1,x2,y2,c); },
             py::arg("x0"),py::arg("y0"),py::arg("x1"),py::arg("y1"),
             py::arg("x2"),py::arg("y2"),py::arg("color"))
        .def("fill_triangle",
             [](GFXDisplay& d,
                int16_t x0,int16_t y0,int16_t x1,int16_t y1,int16_t x2,int16_t y2,
                uint16_t c){ d.fillTriangle(x0,y0,x1,y1,x2,y2,c); },
             py::arg("x0"),py::arg("y0"),py::arg("x1"),py::arg("y1"),
             py::arg("x2"),py::arg("y2"),py::arg("color"))

        // text
        .def("set_cursor",
             [](GFXDisplay& d, int16_t x, int16_t y){ d.setCursor(x,y); },
             py::arg("x"), py::arg("y"))
        .def("set_text_color",
             [](GFXDisplay& d, uint16_t c){ d.setTextColor(c); },
             py::arg("color"))
        .def("set_text_color_bg",
             [](GFXDisplay& d, uint16_t fg, uint16_t bg){ d.setTextColor(fg,bg); },
             py::arg("color"), py::arg("bg"))
        .def("set_text_size",
             [](GFXDisplay& d, uint8_t s){ d.setTextSize(s); },
             py::arg("size"))
        .def("set_text_wrap",
             [](GFXDisplay& d, bool w){ d.setTextWrap(w); },
             py::arg("wrap"))
        .def("print",
             [](GFXDisplay& d, const std::string& s){ d.print(s.c_str()); },
             py::arg("text"))
        .def("println",
             [](GFXDisplay& d, const std::string& s){ d.println(s.c_str()); },
             py::arg("text"))

        // ---- get_text_bounds -----------------------------------------------
        // used a tuple becuase of c++ pointer
        .def("get_text_bounds",
             [](GFXDisplay& d, const std::string& text, int16_t x, int16_t y)
                 -> py::tuple {
                 int16_t x1, y1;
                 uint16_t w, h;
                 d.getTextBounds(text.c_str(), x, y, &x1, &y1, &w, &h);
                 return py::make_tuple(x1, y1, w, h);
             },
             py::arg("text"), py::arg("x") = 0, py::arg("y") = 0,
             "Returns (x1, y1, w, h) bounding box for text at (x, y).")

        // ---- set_font ------------------------------------------------------
        .def("set_font",
             [](GFXDisplay& d, const std::string& name) {
                 if      (name == "FreeMono9"         ) d.setFont(&FreeMono9pt7b);
                 else if (name == "FreeMono12"        ) d.setFont(&FreeMono12pt7b);
                 else if (name == "FreeMono18"        ) d.setFont(&FreeMono18pt7b);
                 else if (name == "FreeMono24"        ) d.setFont(&FreeMono24pt7b);
                 else if (name == "FreeMonoBold9"     ) d.setFont(&FreeMonoBold9pt7b);
                 else if (name == "FreeMonoBold12"    ) d.setFont(&FreeMonoBold12pt7b);
                 else if (name == "FreeSans9"         ) d.setFont(&FreeSans9pt7b);
                 else if (name == "FreeSans12"        ) d.setFont(&FreeSans12pt7b);
                 else if (name == "FreeSans18"        ) d.setFont(&FreeSans18pt7b);
                 else if (name == "FreeSans24"        ) d.setFont(&FreeSans24pt7b);
                 else if (name == "FreeSansBold9"     ) d.setFont(&FreeSansBold9pt7b);
                 else if (name == "FreeSansBold12"    ) d.setFont(&FreeSansBold12pt7b);
                 else if (name == "FreeSansBold18"    ) d.setFont(&FreeSansBold18pt7b);
                 else if (name == "FreeSansBold24"    ) d.setFont(&FreeSansBold24pt7b);
                 else if (name == "FreeSerif9"        ) d.setFont(&FreeSerif9pt7b);
                 else if (name == "FreeSerif12"       ) d.setFont(&FreeSerif12pt7b);
                 else if (name == "FreeSerif18"       ) d.setFont(&FreeSerif18pt7b);
                 else if (name == "FreeSerif24"       ) d.setFont(&FreeSerif24pt7b);
                 else if (name == "Org01"             ) d.setFont(&Org_01);
                 else if (name == "TomThumb"          ) d.setFont(&TomThumb);

                 else if (name == "GeorgiaI12") d.setFont(&georgiai12pt7b);

                 else throw std::invalid_argument("Unknown font: " + name);
             },
             py::arg("name"),
             "Set a named font. See pydafruit_gfx.FONTS for available names.")
        .def("set_font_default",
             [](GFXDisplay& d){ d.setFont(nullptr); },
             "Reset to the built-in 5x7 bitmap font.")

        // bitmap
        .def("draw_bitmap",
             [](GFXDisplay& d, int16_t x, int16_t y, py::bytes data,
                int16_t w, int16_t h, uint16_t color) {
                 std::string buf(data);
                 d.drawBitmap(x, y, reinterpret_cast<const uint8_t*>(buf.data()),
                              w, h, color);
             },
             py::arg("x"),py::arg("y"),py::arg("bitmap"),
             py::arg("w"),py::arg("h"),py::arg("color"))

        // rotation
        .def("set_rotation",
             [](GFXDisplay& d, uint8_t r){ d.setRotation(r); },
             py::arg("rotation"))
        .def_property_readonly("width",  [](GFXDisplay& d){ return d.width();  })
        .def_property_readonly("height", [](GFXDisplay& d){ return d.height(); })
        ;

    // -- font lists -------------------------
    m.attr("FONTS") = py::list(py::cast(std::vector<std::string>{
        "FreeMono9","FreeMono12","FreeMono18","FreeMono24",
        "FreeMonoBold9","FreeMonoBold12",
        "FreeSans9","FreeSans12","FreeSans18","FreeSans24",
        "FreeSansBold9","FreeSansBold12","FreeSansBold18","FreeSansBold24",
        "FreeSerif9","FreeSerif12","FreeSerif18","FreeSerif24",
        "Org01","TomThumb","GeorgiaI12"
    }));
}