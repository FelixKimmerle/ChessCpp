#pragma once
#include <cstdint>
#include "Mask.hpp"

const Mask RANK_1 = 0x00000000000000FFull;
const Mask RANK_2 = 0x000000000000FF00ull;
const Mask RANK_3 = 0x0000000000FF0000ull;
const Mask RANK_4 = 0x00000000FF000000ull;
const Mask RANK_5 = 0x000000FF00000000ull;
const Mask RANK_6 = 0x0000FF0000000000ull;
const Mask RANK_7 = 0x00FF000000000000ull;
const Mask RANK_8 = 0xFF00000000000000ull;

const Mask FILE_A = 0x0101010101010101ull;
const Mask FILE_B = 0x0202020202020202ull;
const Mask FILE_C = 0x0404040404040404ull;
const Mask FILE_D = 0x0808080808080808ull;
const Mask FILE_E = 0x1010101010101010ull;
const Mask FILE_F = 0x2020202020202020ull;
const Mask FILE_G = 0x4040404040404040ull;
const Mask FILE_H = 0x8080808080808080ull;

const Mask WHITE_SQUARES = 0x55AA55AA55AA55AAull;
const Mask BLACK_SQUARES = 0xAA55AA55AA55AA55ull;

const Mask LONG_DIAGONALS = 0x8142241818244281ull;
const Mask CENTER_SQUARES = 0x0000001818000000ull;
const Mask CENTER_BIG = 0x00003C3C3C3C0000ull;

const Mask LEFT_FLANK = FILE_A | FILE_B | FILE_C | FILE_D;
const Mask RIGHT_FLANK = FILE_E | FILE_F | FILE_G | FILE_H;

const Mask PROMOTION_RANKS = RANK_1 | RANK_8;

const unsigned int PAWN_BITBOARD_POSITION = 0;
const unsigned int ROOK_BITBOARD_POSITION = 1;
const unsigned int KNIGHT_BITBOARD_POSITION = 2;
const unsigned int BISHOP_BITBOARD_POSITION = 3;
const unsigned int QUEEN_BITBOARD_POSITION = 4;
const unsigned int KING_BITBOARD_POSITION = 5;
const unsigned int ALL_BITBOARD_POSITION = 6;
const unsigned int NUMBER_BITBOARDS = 7;

const unsigned int NUMBER_PLAYERS = 2;
const uint8_t COLOR_WHITE = 0;
const uint8_t COLOR_BLACK = 1;

enum class Pieces : uint8_t
{
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
};

const char characters[] = {'P', 'R', 'N', 'B', 'Q', 'K'};
