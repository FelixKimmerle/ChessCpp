#pragma once
#include <cstdint>
#include <ostream>

enum Color : uint8_t
{
    White,
    Black,
    NoColor,
};

Color other_color(Color color);
char to_char(Color color);