#include "StateInfo.hpp"

StateInfo::StateInfo(/* args */) : castlingFlags(WhiteKingside | WhiteQueenside | BlackKingside | BlackQueenside), enpasant(), captured_piece(Empty), fifty_move_counter(0)
{
}
void StateInfo::set_captured_piece(Piece piece)
{
    captured_piece = piece;
}

Piece StateInfo::get_captured_piece() const
{
    return captured_piece;
}