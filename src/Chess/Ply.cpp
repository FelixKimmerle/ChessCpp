#include "Ply.hpp"

Ply::Ply(Piece piece, Location source, Location destination) : piece(piece),
                                                               source(source),
                                                               destination(destination),
                                                               ply_type(PlyType::Normal)
{
}

Ply::Ply(Piece piece, Location source, Location destination, PlyType ply_type) : piece(piece),
                                                                                 source(source),
                                                                                 destination(destination),
                                                                                 ply_type(ply_type)
{
}

Ply::Ply(Piece piece, Location source, Location destination, Piece promotion_piece) : piece(piece),
                                                                                      source(source),
                                                                                      destination(destination),
                                                                                      ply_type(static_cast<PlyType>(promotion_piece))
{
    assert(static_cast<uint8_t>(promotion_piece) <= static_cast<uint8_t>(Piece::Rook));
}

Location Ply::get_source() const
{
    return source;
}
Location Ply::get_destination() const
{
    return destination;
}
Piece Ply::get_piece() const
{
    return piece;
}

bool Ply::is_promotion() const
{
    return static_cast<uint8_t>(ply_type) <= static_cast<uint8_t>(PlyType::PromoteRook);
}

Piece Ply::get_promotion_pice() const
{
    assert(is_promotion());
    return static_cast<Piece>(ply_type);
}
PlyType Ply::get_ply_type() const
{
    return ply_type;
}

std::ostream &operator<<(std::ostream &os, const Ply &ply)
{
    os << characters[static_cast<size_t>(ply.piece)] << " " << ply.source << "-" << ply.destination;
    if (ply.is_promotion())
    {
        os << " = " << characters[static_cast<size_t>(ply.get_promotion_pice())];
    }
    return os;
}

bool Ply::operator==(const Ply &other)const
{
    return (piece == other.piece) && (source == other.source) && (destination == other.destination) && (ply_type == other.ply_type);
}