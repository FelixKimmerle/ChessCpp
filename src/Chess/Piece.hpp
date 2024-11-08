#pragma once

#include <cstdint>

enum Piece : uint8_t
{
    Queen,
    Knight,
    Bishop,
    Rook,
    Pawn,
    King,
    Empty,
};

const char characters[] = {'Q', 'N', 'B', 'R', 'P', 'K', 'A'};

extern const char* pieceNames[];
constexpr int pieceNamesSize = 8;
