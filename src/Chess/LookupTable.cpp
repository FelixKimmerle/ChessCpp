#include <iostream>

#include "LookupTable.hpp"
#include "Color.hpp"

LookupTable::LookupTable(/* args */)
{
    init_ray_attacks();
    init_knight_attacks();
    init_king_attacks();
    init_pawn_attacks();
}


//TODO use more lookup tabels to make rotation redundand
Mask LookupTable::get_rook_attack(Mask occupied, Mask anti_flipped, Mask own, Location position)const
{
    uint8_t file = position.get_file();
    Line occupied_file = anti_flipped.get_line(file);

    uint8_t rank = position.get_rank();
    Line occupied_rank = occupied.get_line(rank);

    Line file_attacks_line = ray_attacks[rank][trim(occupied_file.get_raw())];
    Mask file_attacks = Mask(file_attacks_line, file).flip_anti_diag();

    Line rank_attacks_line = ray_attacks[file][trim(occupied_rank.get_raw())];
    Mask rank_attacks(rank_attacks_line, rank);

    return (file_attacks | rank_attacks) & (~own);
}

Mask LookupTable::get_bishop_attack(Mask occupied_left, Mask occupied_right, Mask own, Location position)const
{

    //std::cout << occupied_left << std::endl;
    //std::cout << occupied_right << std::endl;

    uint8_t diag = position.get_diagonal();
    uint8_t anti_diag = position.get_anti_diagonal();

    //std::cout << "diag: " << (int)diag << std::endl;
    //std::cout << "anti diag: " << (int)anti_diag << std::endl;

    Line diag_occupied = occupied_left.get_diagonal(diag);
    Line anti_diag_occupied = occupied_right.get_anti_diagonal(anti_diag);

    //std::cout << "diag_occupied: " << std::endl << diag_occupied << std::endl;
    //std::cout << "anti_diag_occupied: " << std::endl << anti_diag_occupied << std::endl;

    uint8_t diag_pos = position.get_file();
    uint8_t anti_diag_pos = position.get_file();

    //std::cout << (int)diag_pos << std::endl;
    //std::cout << (int)anti_diag_pos << std::endl;

    Line diag_attacks = (ray_attacks[diag_pos][trim(diag_occupied.get_raw())] & MaskTable[diag]);
    Line anti_diag_attacks = ((ray_attacks[anti_diag_pos][trim(anti_diag_occupied.get_raw())] & MaskTable[14 - anti_diag]));

    //std::cout << "diag_attacks: " << std::endl << diag_attacks << std::endl;
    //std::cout << "anti_diag_attacks: " << std::endl << anti_diag_attacks << std::endl;

    Mask diag_mask = Mask(diag_attacks, OffsetTable[diag]);
    Mask anti_diag_mask = Mask(anti_diag_attacks, OffsetTable[anti_diag]);

    //std::cout << "diag_mask: " << std::endl << diag_mask << std::endl;
    //std::cout << "anti_diag_mask: " << std::endl << anti_diag_mask << std::endl;

    //std::cout << (int)diag << std::endl;
    //std::cout << (int)anti_diag << std::endl;

    diag_mask = diag_mask.rotate_right_45();
    anti_diag_mask = anti_diag_mask.rotate_left_45();

    //std::cout << "diag_mask rot: " << std::endl << diag_mask << std::endl;
    //std::cout << "anti_diag_mask rot: " << std::endl << anti_diag_mask << std::endl;

    return (diag_mask | anti_diag_mask) & (~own);
}

Mask LookupTable::get_knight_attack(Mask own, Location position)const
{
    return knight_attacks[position.get_square()] & (~own);
}
Mask LookupTable::get_king_attack(Mask own, Location position)const
{
    return king_attacks[position.get_square()] & (~own);
}

Mask LookupTable::get_pawn_attacks(uint8_t color, Mask enemy_occupied, Location position)const
{
    return pawn_attacks[color][position.get_square()] & enemy_occupied;
}

uint8_t LookupTable::trim(uint8_t value)const
{
    return (value & 0b01111110) >> 1;
}

void LookupTable::init_king_attacks()
{
    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            Location location(x, y);
            Mask king = location.get_mask();
            Mask attacks = king.eastOne() | king.westOne();
            king |= attacks;
            attacks |= king.nortOne() | king.soutOne();
            //std::cout << location << std::endl;
            //std::cout << attacks << std::endl;
            king_attacks[location.get_square()] = attacks;
        }
    }
}

void LookupTable::init_knight_attacks()
{
    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            Location location(x, y);
            uint64_t knight = location.get_mask().get_raw();
            uint64_t l1 = (knight >> 1) & (0x7f7f7f7f7f7f7f7f);
            uint64_t l2 = (knight >> 2) & (0x3f3f3f3f3f3f3f3f);
            uint64_t r1 = (knight << 1) & (0xfefefefefefefefe);
            uint64_t r2 = (knight << 2) & (0xfcfcfcfcfcfcfcfc);
            uint64_t h1 = l1 | r1;
            uint64_t h2 = l2 | r2;
            Mask mask((h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8));

            //std::cout << location << std::endl;
            //std::cout << mask << std::endl;
            knight_attacks[location.get_square()] = mask;
        }
    }
}

void LookupTable::init_pawn_attacks()
{
    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            Location location(x, y);
            Mask location_mask = location.get_mask();
            Mask white;
            if (x != 0)
            {
                white |= location_mask << 7;
            }
            if (x != 7)
            {
                white |= location_mask << 9;
            }
            Mask black;
            if (x != 7)
            {
                black |= location_mask >> 7;
            }
            if (x != 0)
            {
                black |= location_mask >> 9;
            }

            pawn_attacks[White][location.get_square()] = white;
            pawn_attacks[Black][location.get_square()] = black;
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
