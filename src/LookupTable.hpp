#pragma once
#include "MultiDimArray.hpp"
#include "Line.hpp"
#include "Mask.hpp"
class LookupTable
{
private:
    MultiDimArray<Line, 8, 64> attacks;
    MultiDimArray<Line, 8, 64> diag_attacks;

    uint8_t trim(uint8_t value);

    void init_attacks();
    void init_diag_attacks();

public:
    LookupTable(/* args */);
    ~LookupTable();

    Mask get_rook_attack(Mask occupied, Mask rotated_occupied, Mask own, Location position);
};
