#pragma once
#include <cstdint>
#include "Location.hpp"
#include "Definitions.hpp"

class Ply
{
private:
    Location source;
    Location destination;
    uint8_t promotion_piece: 2;
    Pieces piece: 3;
    uint8_t takes: 1;
    uint8_t check: 1;
public:
    Ply(Location source, Location destination);
    ~Ply();

    friend std::ostream &operator<<(std::ostream &os, const Ply &ply);
};

