#include <gfx_cpp14.hpp>

#include "config.hpp"
//#include "ummm.hpp"
//#include "frankenstein_jpg.hpp"
#include "frankenstein.hpp"
using namespace gfx;
lcd_type lcd;

using lcd_color = color<decltype(lcd)::pixel_type>;
const char *text =
    "GFX Demo - Copyright (C) 2022 by honey the codewitch - MIT license  ";

void setup() {
    Serial.begin(115200);
    draw::filled_rectangle(lcd,lcd.bounds(),lcd_color::white);
    draw::bitmap(lcd,lcd.bounds(),frankenstein,frankenstein.bounds());
    //draw::image(lcd,lcd.bounds(),&frankenstein);
}
void loop() {
}
