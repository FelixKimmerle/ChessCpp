#include "Ply.hpp"

Ply::Ply(Location source, Location destination) : source(source), destination(destination)
{
}

Ply::~Ply()
{
}

std::ostream &operator<<(std::ostream &os, const Ply &ply)
{
    os << characters[static_cast<size_t>(ply.piece)] << " " << ply.source << "-" << ply.destination;
    return os;
}