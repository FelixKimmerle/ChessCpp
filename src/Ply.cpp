#include "Ply.hpp"

Ply::Ply(Piece piece, Location source, Location destination) : piece(piece), source(source), destination(destination)
{
}

Ply::~Ply()
{
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
uint8_t Ply::get_piece_index() const
{
    return static_cast<uint8_t>(piece);
}

std::ostream &operator<<(std::ostream &os, const Ply &ply)
{
    os << characters[static_cast<size_t>(ply.piece)] << " " << ply.source << "-" << ply.destination;
    return os;
}