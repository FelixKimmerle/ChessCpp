#pragma once
#include "MultiDimArray.hpp"
#include "Line.hpp"
#include "Mask.hpp"
class LookupTable
{
private:
    MultiDimArray<Line, 8, 64> ray_attacks;
    MultiDimArray<Mask, 64> knight_attacks;
    MultiDimArray<Mask, 64> king_attacks;
    MultiDimArray<Mask, 2, 64> pawn_attacks;

    uint8_t trim(uint8_t value) const;

    void init_ray_attacks();
    void init_knight_attacks();
    void init_king_attacks();
    void init_pawn_attacks();

public:
    LookupTable(/* args */);

    Mask get_rook_attack(Mask occupied, Mask anti_flipped, Mask own, Location position) const;
    Mask get_bishop_attack(Mask rotated_left, Mask rotated_right, Mask own, Location position) const;
    Mask get_knight_attack(Mask own, Location position) const;
    Mask get_king_attack(Mask own, Location position) const;
    Mask get_pawn_attacks(uint8_t color, Mask enemy_occupied, Location position) const;
    Mask get_pawn_attacks(uint8_t color, Location position) const;
};
