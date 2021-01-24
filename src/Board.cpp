#include "Board.hpp"
#include "ConsoleColor.hpp"
#include "Location.hpp"
#include <bitset>

#include <iostream>

Board::Board(/* args */)
{
    //pieces[COLOR_WHITE][PAWN_BITBOARD_POSITION] = 0xff00;
    pieces[COLOR_WHITE][PAWN_BITBOARD_POSITION] = 0;
    pieces[COLOR_BLACK][PAWN_BITBOARD_POSITION] = 0;

    pieces[COLOR_WHITE][ROOK_BITBOARD_POSITION] = 0x81;
    pieces[COLOR_WHITE][KNIGHT_BITBOARD_POSITION] = 0x42;
    pieces[COLOR_WHITE][BISHOP_BITBOARD_POSITION] = 0x40224;
    pieces[COLOR_WHITE][QUEEN_BITBOARD_POSITION] = 0x10;
    pieces[COLOR_WHITE][KING_BITBOARD_POSITION] = 0x8;
    //pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] = 0xff;

    for (uint8_t i = ROOK_BITBOARD_POSITION; i <= KING_BITBOARD_POSITION; i++)
    {
        pieces[COLOR_BLACK][i] = pieces[COLOR_WHITE][i] << (8 * 7);
    }

    //pieces[COLOR_BLACK][PAWN_BITBOARD_POSITION] = pieces[COLOR_WHITE][PAWN_BITBOARD_POSITION] << (8 * 5);
    pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] = pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] << (8 * 6);

    for (uint8_t i = 0; i < NUMBER_BITBOARDS; i++)
    {
        //std::cout << pieces[COLOR_WHITE][i] << std::endl;
        Mask curr = pieces[COLOR_WHITE][i];
        //std::cout << curr.more_than_one() << std::endl;
    }

    for (uint8_t i = PAWN_BITBOARD_POSITION; i <= KING_BITBOARD_POSITION; i++)
    {
        pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] |= pieces[COLOR_BLACK][i];
        pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] |= pieces[COLOR_WHITE][i];
    }

    std::cout << *this << std::endl;

    Mask occupied = pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] | pieces[COLOR_WHITE][ALL_BITBOARD_POSITION];
    Mask rotated_occupied = occupied.rotate_right_90();

    Mask rotated_left = occupied.rotate_left_45();
    Mask rotated_right = occupied.rotate_right_45();

    //uint8_t y = 3;
    //for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            std::cout << Location(7-x, x) << std::endl << std::endl;
            Mask moves = table.get_bishop_attack(rotated_left, rotated_right, pieces[COLOR_WHITE][ALL_BITBOARD_POSITION], Location(7-x, x));
            std::cout << moves << std::endl;
        }
    }

    possible_plies.reserve(32);
}

Board::~Board()
{
}

void Board::calculate_possible_plies_pawns()
{
}
void Board::calculate_possible_plies_rook()
{
}
void Board::calculate_possible_plies_knight()
{
}
void Board::calculate_possible_plies_bishop()
{
}
void Board::calculate_possible_plies_queen()
{
}
void Board::calculate_possible_plies_king()
{
}

void Board::background(uint8_t x, uint8_t y, std::ostream &os) const
{
    if ((x + y % 2) % 2 == 0)
    {
        os << Color::Backgound::Grey;
    }
    else
    {
        os << Color::Backgound::Lime;
    }
}

void Board::print_line(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << "       " << Color::Reset;
    }
    os << std::endl;
}

void Board::print_piece(uint8_t x, uint8_t y, std::ostream &os) const
{
    Mask mask = Location(x, y).get_mask();
    uint8_t color = COLOR_WHITE;
    if ((pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] & mask) != 0)
    {
        os << Color::Text::Black;
        color = COLOR_BLACK;
    }
    else if ((pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] & mask) != 0)
    {
        os << Color::Text::White;
        color = COLOR_WHITE;
    }
    else
    {
        os << " ";
        return;
    }

    uint8_t counter = 0;

    while ((pieces[color][counter] & mask) == 0)
    {
        counter++;
    }
    os << characters[counter];
}

void Board::print_line_pieces(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << "   ";
        print_piece(x, y, os);
        os << "   " << Color::Reset;
    }
    os << std::endl;
}

void Board::print_line_position(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << Color::Text::Red << Location(x, y);
        os << "     " << Color::Reset;
    }
    os << std::endl;
}
std::ostream &operator<<(std::ostream &os, const Board &board)
{
    for (uint8_t y = 7; y != 255; y--)
    {
        board.print_line_position(y, os);
        board.print_line_pieces(y, os);
        board.print_line(y, os);
    }
    return os;
}
