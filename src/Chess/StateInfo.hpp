#pragma once
#include <cstdint>

#include "Location.hpp"
#include "Piece.hpp"

enum CastlingFlags : uint8_t
{
    WhiteKingside = 1 << 0,
    WhiteQueenside = 1 << 1,
    BlackKingside = 1 << 2,
    BlackQueenside = 1 << 3,
};

inline CastlingFlags operator|(CastlingFlags a, CastlingFlags b)
{
    return static_cast<CastlingFlags>(static_cast<int>(a) | static_cast<int>(b));
}

class StateInfo
{
private:
    CastlingFlags castlingFlags : 4;
    Location enpasant;
    Piece captured_piece;
    uint32_t fifty_move_counter;
public:
    StateInfo(/* args */);
    void set_captured_piece(Piece piece);
    Piece get_captured_piece()const;

};

