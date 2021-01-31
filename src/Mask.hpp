#pragma once
#include <cstdint>
#include <ostream>
#include "Location.hpp"
#include "Line.hpp"
class Mask
{
private:
    uint64_t mask;

    void background(uint8_t x, uint8_t y, std::ostream &os) const;
    void print_line(uint8_t y, std::ostream &os) const;
    void print_line_values(uint8_t y, std::ostream &os) const;
    void print_line_position(uint8_t y, std::ostream &os) const;

    uint8_t first_one() const;

public:
    Mask(uint64_t mask = 0);
    Mask(Line line, uint8_t position = 0);

    ~Mask();

    Mask operator&(const Mask &other) const;
    Mask operator&=(const Mask &other);
    bool operator==(const Mask &other) const;
    void operator=(const uint64_t other);
    Mask operator<<(const uint8_t other) const;
    Mask operator>>(const uint8_t other) const;
    bool operator!=(const Mask &other) const;
    Mask operator|(const Mask &other) const;
    Mask operator|=(const Mask &other);
    Mask operator~() const;

    uint64_t get_raw() const;

    Location get_next_location();
    bool is_zero() const;
    Mask rotate_bits_right(uint8_t bits) const;
    Mask rotate_bits_left(uint8_t bits) const;
    bool more_than_one() const;
    bool is_set(Location location) const;
    void set(Location location);
    void reset(Location location);
    Line get_line(uint8_t index) const;
    Line get_diagonal(uint8_t index) const;
    Line get_anti_diagonal(uint8_t index) const;

    Mask rotate_right_45() const;
    Mask rotate_left_45() const;
    Mask rotate_right_90() const;
    Mask rotate_left_90() const;
    Mask flip_vertical() const;
    Mask flip_horizontal() const;
    Mask flip_main_diag() const;
    Mask flip_anti_diag() const;

    Mask eastOne() const;
    Mask noEaOne() const;
    Mask soEaOne() const;
    Mask westOne() const;
    Mask soWeOne() const;
    Mask noWeOne() const;

    Mask soutOne() const;
    Mask nortOne() const;

    friend std::ostream &operator<<(std::ostream &os, const Mask &mask);
};
