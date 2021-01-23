#include "Line.hpp"
#include "ConsoleColor.hpp"

Line::Line(uint8_t data) : data(data)
{
}

Line::Line() : data(0)
{
}

Line::~Line()
{
}

uint8_t Line::get_raw() const
{
    return data;
}

bool Line::is_set(uint8_t bit_position) const
{
    return (data >> bit_position) & uint8_t(1);
}

void Line::set(uint8_t bit_position)
{
    data |= (1 << bit_position);
}

Line Line::operator|(const Line &other) const
{
    return data | other.data;
}
Line Line::operator|=(const Line &other)
{
    return data |= other.data;
}

Line Line::operator&(const Line &other) const
{
    return data & other.data;
}
Line Line::operator&=(const Line &other)
{
    return data &= other.data;
}

Line Line::operator^(const Line &other) const
{
    return data ^ other.data;
}
Line Line::operator^=(const Line &other)
{
    return data ^= other.data;
}

Line Line::operator>>(const Line &other) const
{
    return data >> other.data;
}

Line Line::operator>>=(const Line &other)
{
    return data >>= other.data;
}
Line Line::operator<<(const Line &other) const
{
    return data << other.data;
}
Line Line::operator<<=(const Line &other)
{
    return data <<= other.data;
}

void Line::background(uint8_t x, std::ostream &os) const
{
    if (is_set(x))
    {
        os << Color::Backgound::White;
    }
    else
    {
        os << Color::Backgound::Black;
    }
}
void Line::print_line(std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, os);
        os << "       " << Color::Reset;
    }
    os << std::endl;
}
void Line::print_line_values(std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, os);
        os << "   ";
        if (is_set(x))
        {
            os << Color::Text::Black << "1";
        }
        else
        {
            os << Color::Text::White << "0";
        }
        os << "   " << Color::Reset;
    }
    os << std::endl;
}
void Line::print_line_position(std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, os);
        os << Color::Text::Aqua << (int)x;
        os << "      " << Color::Reset;
    }
    os << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Line &line)
{
    line.print_line_position(os);
    line.print_line_values(os);
    line.print_line(os);
    return os;
}