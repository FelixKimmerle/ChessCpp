#pragma once
#include "MultiDimArray.hpp"
#include "Line.hpp"
#include "Mask.hpp"
class LookupTable
{
private:
    MultiDimArray<Line, 8, 64> ray_attacks;
    MultiDimArray<Mask, 64> knight_attacks;

    uint8_t trim(uint8_t value);

    void init_ray_attacks();
    void init_knight_attacks();

public:
    LookupTable(/* args */);
    ~LookupTable();

    Mask get_rook_attack(Mask occupied, Mask rotated_occupied, Mask own, Location position);
    Mask get_bishop_attack(Mask occupied_left, Mask rotated_right, Mask own, Location position);
    Mask get_queen_attack(Mask occupied, Mask own, Location position);
};
