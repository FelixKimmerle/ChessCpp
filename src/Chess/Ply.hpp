#pragma once

#include <cassert>

#include "Location.hpp"
#include "Piece.hpp"

enum class PlyType
{
    PromoteQueen,
    PromoteKnight,
    PromoteBishop,
    PromoteRook,
    EnPassant,
    Castling,
    PawnDoublePush,
    Normal,
};

class Ply
{
private:
    Piece piece : 3;
    Location source;
    Location destination;
    PlyType ply_type : 3;

public:
    Ply(Piece piece, Location source, Location destination);
    Ply(Piece piece, Location source, Location destination, PlyType ply_type);
    Ply(Piece piece, Location source, Location destination, Piece promotion_piece);

    Location get_source() const;
    Location get_destination() const;
    Piece get_piece() const;
    bool is_promotion() const;
    Piece get_promotion_pice() const;
    PlyType get_ply_type() const;

    friend std::ostream &operator<<(std::ostream &os, const Ply &ply);

    bool operator==(const Ply & other)const;
};
