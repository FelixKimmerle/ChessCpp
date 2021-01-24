#include "LookupTable.hpp"
#include <iostream>
#include "Definitions.hpp"

LookupTable::LookupTable(/* args */)
{
    init_ray_attacks();
    init_knight_attacks();
}

LookupTable::~LookupTable()
{
}
#include <iostream>
Mask LookupTable::get_rook_attack(Mask occupied, Mask rotated_occupied, Mask own, Location position)
{
    std::cout << position << std::endl;
    std::cout << "Occupied: " << std::endl
              << occupied << std::endl;
    std::cout << "Rotated: " << std::endl
              << rotated_occupied << std::endl;

    uint8_t file = 7 - position.get_x();
    std::cout << "File number: " << (uint)file << std::endl;
    Line occupied_file = rotated_occupied.get_line(file);
    std::cout << "Occupied file: " << std::endl
              << occupied_file << std::endl;

    uint8_t rank = position.get_y();
    std::cout << "Rank number: " << (uint)file << std::endl;
    Line occupied_rank = occupied.get_line(rank);
    std::cout << "Occupied rank: " << std::endl
              << occupied_rank << std::endl;

    Mask file_attacks = Mask(ray_attacks[file][trim(occupied_file.get_raw())], file).rotate_left_90();
    std::cout << "file_attacks" << std::endl
              << file_attacks << std::endl;

    Mask rank_attacks(ray_attacks[7 - rank][trim(occupied_rank.get_raw())], rank);
    std::cout << "rank_attacks" << std::endl
              << rank_attacks << std::endl;

    return (file_attacks | rank_attacks) & (~own);
}

Mask LookupTable::get_bishop_attack(Mask occupied_left, Mask occupied_right, Mask own, Location position)
{

    //std::cout << occupied_left << std::endl;
    //std::cout << occupied_right << std::endl;

    uint8_t diag = position.get_diagonal();
    uint8_t anti_diag = position.get_anti_diagonal();

    //std::cout << (int)diag << std::endl;
    //std::cout << (int)anti_diag << std::endl;

    Line diag_occupied = occupied_left.get_diagonal(diag);
    Line anti_diag_occupied = occupied_right.get_anti_diagonal(anti_diag);

    //std::cout << diag_occupied << std::endl;
    //std::cout << anti_diag_occupied << std::endl;

    uint8_t diag_pos = position.get_x();
    uint8_t anti_diag_pos = position.get_x();

    //std::cout << (int)diag_pos << std::endl;
    //std::cout << (int)anti_diag_pos << std::endl;
    Line diag_attacks = (ray_attacks[diag_pos][trim(diag_occupied.get_raw())] & MaskTable[diag]);
    Line anti_diag_attacks = ((ray_attacks[anti_diag_pos][trim(anti_diag_occupied.get_raw())] & MaskTable[14 - anti_diag]));

    //std::cout << diag_attacks << std::endl;
    //std::cout << anti_diag_attacks << std::endl;

    Mask diag_mask = Mask(diag_attacks, OffsetTable[diag]);
    Mask anti_diag_mask = Mask(anti_diag_attacks, OffsetTable[anti_diag]);

    //std::cout << (int)diag << std::endl;
    //std::cout << (int)anti_diag << std::endl;

    diag_mask = diag_mask.rotate_right_45();
    anti_diag_mask = anti_diag_mask.rotate_left_45();

    //std::cout << diag_mask << std::endl;
    //std::cout << anti_diag_mask << std::endl;

    return (diag_mask | anti_diag_mask) & ~(own);
}

uint8_t LookupTable::trim(uint8_t value)
{
    return (value & 0b01111110) >> 1;
}

void LookupTable::init_knight_attacks()
{
    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            Location location(x,y);
            uint64_t knight = location.get_mask().get_raw();
            uint64_t l1 = (knight >> 1) & (0x7f7f7f7f7f7f7f7f);
            uint64_t l2 = (knight >> 2) & (0x3f3f3f3f3f3f3f3f);
            uint64_t r1 = (knight << 1) & (0xfefefefefefefefe);
            uint64_t r2 = (knight << 2) & (0xfcfcfcfcfcfcfcfc);
            uint64_t h1 = l1 | r1;
            uint64_t h2 = l2 | r2;
            Mask mask((h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8));

            std::cout << location << std::endl;
            std::cout << mask << std::endl;
            knight_attacks[location.get_number()] = mask;
        }
    }
}

void LookupTable::init_ray_attacks()
{
    for (uint16_t i = 0; i != 64; i++)
    {
        Line line(i);

        for (uint8_t position = 0; position < 8; position++)
        {
            Line attack;
            if (position != 0)
            {
                uint8_t currpos = position - 1;
                while (currpos != 255)
                {
                    if (line.is_set(currpos - 1))
                    {
                        attack.set(currpos);
                        break;
                    }
                    attack.set(currpos);
                    currpos--;
                }
            }

            if (position != 7)
            {
                uint8_t currpos = position + 1;
                while (currpos != 8)
                {
                    if (line.is_set(currpos - 1))
                    {
                        attack.set(currpos);
                        break;
                    }
                    attack.set(currpos);
                    currpos++;
                }
            }

            ray_attacks[position][line.get_raw()] = attack;
        }
    }
}
