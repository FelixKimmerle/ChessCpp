#include <cassert>

#include "Location.hpp"
#include "Mask.hpp"

Location::Location() : square(0b10000000)
{
}

Location::Location(uint8_t number) : square(number)
{
    assert(number < 64);
}

Location::Location(uint8_t file, uint8_t rank) : square(file | (rank << 3))
{
    assert(square < 64);
}

Location::Location(const char name[2]) : square((name[0] - 'a') | ((name[1] - '1') << 3))
{
    assert(square < 64);
}

bool Location::is_valid() const
{
    return square != 0b10000000;
}

Mask Location::get_mask() const
{
    assert(is_valid());
    return (uint64_t)1 << square;
}

uint8_t Location::get_file() const
{
    assert(is_valid());
    return square & 0b00000111;
}
uint8_t Location::get_rank() const
{
    assert(is_valid());
    return square >> 3;
}

char Location::get_file_char() const
{
    assert(is_valid());
    return get_file() + 'a';
}
char Location::get_rank_char() const
{
    assert(is_valid());
    return get_rank() + '1';
}

uint8_t Location::get_diagonal() const
{
    assert(is_valid());
    return get_file() + get_rank();
}
uint8_t Location::get_anti_diagonal() const
{
    assert(is_valid());
    return (7 - get_file()) + get_rank();
}

uint8_t Location::get_square() const
{
    assert(is_valid());
    return square;
}

bool Location::operator==(const Location &other) const
{
    return (square == other.square);
}

bool Location::operator!=(const Location &other) const
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Location &location)
{
    if (location.is_valid())
    {
        os << location.get_file_char() << location.get_rank_char();
    }
    else
    {
        os << "(invalid)";
    }
    return os;
}
