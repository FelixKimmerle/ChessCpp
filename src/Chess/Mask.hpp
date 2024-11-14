#pragma once
#include <cstdint>
#include <ostream>
#include "Location.hpp"
#include "Line.hpp"
#include "MultiDimArray.hpp"

static const unsigned char MaskTable[] = {
    0b00000001,
    0b00000011,
    0b00000111,
    0b00001111,
    0b00011111,
    0b00111111,
    0b01111111,
    0b11111111,
    0b11111110,
    0b11111100,
    0b11111000,
    0b11110000,
    0b11100000,
    0b11000000,
    0b10000000,
};

static const unsigned char OffsetTable[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
};

static const unsigned char ShiftTable[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
};

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
    uint8_t count_ones() const;
    bool is_set(Location location) const;
    void set(Location location);
    void reset(Location location);
    Line get_line(uint8_t index) const;
    Line get_column(uint8_t index)const;
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


static const Mask RANK_1 = 0x00000000000000FFull;
static const Mask RANK_2 = 0x000000000000FF00ull;
static const Mask RANK_3 = 0x0000000000FF0000ull;
static const Mask RANK_4 = 0x00000000FF000000ull;
static const Mask RANK_5 = 0x000000FF00000000ull;
static const Mask RANK_6 = 0x0000FF0000000000ull;
static const Mask RANK_7 = 0x00FF000000000000ull;
static const Mask RANK_8 = 0xFF00000000000000ull;


const Mask FILE_A = 0x0101010101010101ull;
const Mask FILE_B = 0x0202020202020202ull;
const Mask FILE_C = 0x0404040404040404ull;
const Mask FILE_D = 0x0808080808080808ull;
const Mask FILE_E = 0x1010101010101010ull;
const Mask FILE_F = 0x2020202020202020ull;
const Mask FILE_G = 0x4040404040404040ull;
const Mask FILE_H = 0x8080808080808080ull;


static const Mask ranks[] = {
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
};

static const Mask files[] = {
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,
};




static const Mask WHITE_SQUARES = 0x55AA55AA55AA55AAull;
static const Mask BLACK_SQUARES = 0xAA55AA55AA55AA55ull;

static const Mask LONG_DIAGONALS = 0x8142241818244281ull;
static const Mask CENTER_SQUARES = 0x0000001818000000ull;
static const Mask CENTER_BIG = 0x00003C3C3C3C0000ull;

static const Mask LEFT_FLANK = FILE_A | FILE_B | FILE_C | FILE_D;
static const Mask RIGHT_FLANK = FILE_E | FILE_F | FILE_G | FILE_H;

static const Mask PROMOTION_RANKS = RANK_1 | RANK_8;