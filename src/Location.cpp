#include "Location.hpp"
#include <bitset>
#include <iostream>
#include "ConsoleColor.hpp"
#include "Mask.hpp"
#include <cassert>
#include "Definitions.hpp"

Location::Location(uint8_t number)
{
    assert(number < 64);
    x = number & 0b00000111;
    y = number >> 3;
}

Location::Location(uint8_t x, uint8_t y) : x(x), y(y)
{
}

Location::Location(const char name[2]) : x(name[0] - 'a'), y(name[1] - '1')
{
}

Location::~Location()
{
}

Mask Location::get_mask() const
{
    return (uint64_t)1 << get_number();
}
uint8_t Location::get_file() const
{
    return x;
}
uint8_t Location::get_rank() const
{
    return y;
}
uint8_t Location::get_file_flipped() const
{
    return 7 - x;
}
uint8_t Location::get_rank_flipped() const
{
    return 7 - y;
}

uint8_t Location::get_number() const
{
    return (y << 3) + x;
}
char Location::get_file_char() const
{
    return x + 'a';
}
char Location::get_rank_char() const
{
    return y + '1';
}

uint8_t Location::get_diagonal() const
{
    return x + y;
}
uint8_t Location::get_anti_diagonal() const
{
    return (7 - x) + y;
}

bool Location::operator==(const Location &other) const
{
    return (x == other.x) && (y == other.y);
}

bool Location::operator!=(const Location &other) const
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Location &location)
{
    os << location.get_file_char() << location.get_rank_char();
    return os;
}
