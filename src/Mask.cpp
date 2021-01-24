#include "Mask.hpp"
#include "ConsoleColor.hpp"
#include <cassert>
#include <cmath>
#include "Definitions.hpp"

Mask::Mask(uint64_t mask) : mask(mask)
{
}

Mask::Mask(Line line, uint8_t position)
{
    assert(position < 8);
    mask = line.get_raw();
    mask <<= (position << 3);
}

Mask::~Mask()
{
}

Mask Mask::operator&(const Mask &other) const
{
    return mask & other.mask;
}
Mask Mask::operator&=(const Mask &other)
{
    mask &= other.mask;
    return *this;
}

bool Mask::operator==(const Mask &other) const
{
    return mask == other.mask;
}

void Mask::operator=(const uint64_t other)
{
    mask = other;
}

Mask Mask::operator<<(const uint64_t other) const
{
    return mask << other;
}

bool Mask::operator!=(const Mask &other) const
{
    return mask != other.mask;
}

Mask Mask::operator|(const Mask &other) const
{
    return mask | other.mask;
}

Mask Mask::operator|=(const Mask &other)
{
    return mask |= other.mask;
}

Mask Mask::operator~() const
{
    return ~mask;
}

uint64_t Mask::get_raw() const
{
    return mask;
}

uint8_t Mask::first_one() const
{
    assert(!is_zero());
    return __builtin_ctzll(mask);
}

bool Mask::more_than_one() const
{
    return mask & (mask - 1);
}

bool Mask::is_set(Location location) const
{
    return (mask >> location.get_number()) & uint8_t(1);
}

void Mask::set(Location location)
{
    mask |= (1ull << location.get_number());
}

Line Mask::get_line(uint8_t index) const
{
    assert(index < 8);
    return (mask >> (index << 3)) & UINT8_MAX;
}
#include <iostream>

Line Mask::get_diagonal(uint8_t index) const
{
    assert(index < 15);
    Line line = get_line(OffsetTable[index + 1]);
    return (line & MaskTable[index]) >> ShiftTable[index];
}

Line Mask::get_anti_diagonal(uint8_t index) const
{
    assert(index < 15);
    Line line = get_line(OffsetTable[index + 1]);
    return (line & MaskTable[14 - index]) >> ShiftTable[14 - index];
}

Location Mask::get_next_location()
{
    uint8_t number = first_one();
    mask ^= 1ull << number;
    return number;
}
bool Mask::is_zero() const
{
    return mask == 0;
}

inline uint64_t rotate_right(uint64_t x, uint8_t bits)
{
    return (x >> bits) | (x << (64 - bits));
}

Mask Mask::rotate_right_45() const
{
    const uint64_t k1 = 0xAAAAAAAAAAAAAAAA;
    const uint64_t k2 = 0xCCCCCCCCCCCCCCCC;
    const uint64_t k4 = 0xF0F0F0F0F0F0F0F0;
    uint64_t result = mask;
    result ^= k1 & (result ^ rotate_right(result, 8));
    result ^= k2 & (result ^ rotate_right(result, 16));
    result ^= k4 & (result ^ rotate_right(result, 32));
    return result;
}
Mask Mask::rotate_left_45() const
{
    const uint64_t k1 = 0x5555555555555555;
    const uint64_t k2 = 0x3333333333333333;
    const uint64_t k4 = 0x0f0f0f0f0f0f0f0f;
    uint64_t result = mask;
    result ^= k1 & (result ^ rotate_right(result, 8));
    result ^= k2 & (result ^ rotate_right(result, 16));
    result ^= k4 & (result ^ rotate_right(result, 32));
    return result;
}
Mask Mask::rotate_right_90() const
{
    return flip_main_diag().flip_vertical();
}

Mask Mask::rotate_left_90() const
{
    return flip_vertical().flip_main_diag();
}

Mask Mask::flip_vertical() const
{
    return __builtin_bswap64(mask);
}
Mask Mask::flip_main_diag() const
{
    uint64_t t;
    const uint64_t k1 = 0x5500550055005500;
    const uint64_t k2 = 0x3333000033330000;
    const uint64_t k4 = 0x0f0f0f0f00000000;
    uint64_t result = mask;
    t = k4 & (result ^ (result << 28));
    result ^= t ^ (t >> 28);
    t = k2 & (result ^ (result << 14));
    result ^= t ^ (t >> 14);
    t = k1 & (result ^ (result << 7));
    result ^= t ^ (t >> 7);
    return result;
}

Mask Mask::flip_anti_diag() const
{
    uint64_t t;
    const uint64_t k1 = 0xaa00aa00aa00aa00;
    const uint64_t k2 = 0xcccc0000cccc0000;
    const uint64_t k4 = 0xf0f0f0f00f0f0f0f;
    uint64_t result = mask;
    t = result ^ (result << 36);
    result ^= k4 & (t ^ (result >> 36));
    t = k2 & (result ^ (result << 18));
    result ^= t ^ (t >> 18);
    t = k1 & (result ^ (result << 9));
    result ^= t ^ (t >> 9);
    return result;
}

void Mask::background(uint8_t x, uint8_t y, std::ostream &os) const
{
    if (is_set(Location(x, y)))
    {
        os << Color::Backgound::White;
        /*
        if ((x + y % 2) % 2 == 0)
        {
            os << Color::Backgound::Silver;
        }
        else
        {
            os << Color::Backgound::White;
        }
        */
    }
    else
    {
        os << Color::Backgound::Black;
        /*
        if ((x + y % 2) % 2 == 0)
        {
            os << Color::Backgound::Black;
        }
        else
        {
            os << Color::Compatibility::Background::DarkGray;
        }
        */
    }
}

void Mask::print_line(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << "       " << Color::Reset;
    }
    os << std::endl;
}

void Mask::print_line_values(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << "   ";
        if (is_set(Location(x, y)))
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

void Mask::print_line_position(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << Color::Text::Aqua << Location(x, y);
        os << "     " << Color::Reset;
    }
    os << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Mask &mask)
{
    for (uint8_t y = 7; y != 255; y--)
    {
        mask.print_line_position(y, os);
        mask.print_line_values(y, os);
        mask.print_line(y, os);
    }
    return os;
}