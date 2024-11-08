#include "Color.hpp"

Color other_color(Color color)
{
    return static_cast<Color>(color ^ Black);
}

char to_char(Color color)
{
    return (color == White) ? 'w' : 'b';
}

const char* colorNames[] = { "White", "Black", "NoColor" };
