#pragma once

#include <cstdint>
#include <ostream>
class Mask;
class Location
{
private:
    uint8_t square;

public:
    Location();
    Location(uint8_t number);
    Location(uint8_t file, uint8_t rank);
    Location(const char name[2]);

    bool is_valid()const;

    Mask get_mask() const;

    uint8_t get_file() const;
    uint8_t get_rank() const;
    char get_file_char() const;
    char get_rank_char() const;
    uint8_t get_diagonal() const;
    uint8_t get_anti_diagonal() const;
    uint8_t get_square() const;

    bool operator==(const Location &other) const;
    bool operator!=(const Location &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Location &location);
};
