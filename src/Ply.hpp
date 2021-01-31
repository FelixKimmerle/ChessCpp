#pragma once
#include <cstdint>
#include "Location.hpp"
#include "Definitions.hpp"

class Ply
{
private:
    Location source;
    Location destination;
    Piece piece : 3;
    uint8_t takes : 1;
    uint8_t check : 1;
    uint8_t promotion_piece : 2;

public:
    Ply(Piece piece, Location source, Location destination);
    ~Ply();

    Location get_source() const;
    Location get_destination() const;
    Piece get_piece() const;
    uint8_t get_piece_index() const;

    friend std::ostream &operator<<(std::ostream &os, const Ply &ply);
};
