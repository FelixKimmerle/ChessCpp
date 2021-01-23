#include "LookupTable.hpp"

LookupTable::LookupTable(/* args */)
{
    init_attacks();
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

    Mask file_attacks = Mask(attacks[file][trim(occupied_file.get_raw())], file).rotate_left_90();
    std::cout << "file_attacks" << std::endl
              << file_attacks << std::endl;

    Mask rank_attacks(attacks[7 - rank][trim(occupied_rank.get_raw())], rank);
    std::cout << "rank_attacks" << std::endl
              << rank_attacks << std::endl;

    return (file_attacks | rank_attacks) & (~own);
}
#include <iostream>

uint8_t LookupTable::trim(uint8_t value)
{
    return (value & 0b01111110) >> 1;
}

void LookupTable::init_attacks()
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
                    if (line.is_set(currpos-1))
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
                    if (line.is_set(currpos-1))
                    {
                        attack.set(currpos);
                        break;
                    }
                    attack.set(currpos);
                    currpos++;
                }
            }
            
            attacks[position][line.get_raw()] = attack;
        }
    }
}

void LookupTable::init_diag_attacks()
{

}