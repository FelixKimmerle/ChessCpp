#include "Board.hpp"
#include "ConsoleColor.hpp"
#include "Location.hpp"
#include <bitset>

#include <iostream>
#include <cassert>

LookupTable Board::table;

Board::Board(/* args */)
{

    for (size_t i = 0; i < NUMBER_BITBOARDS; i++)
    {
        pieces[COLOR_WHITE][i] = 0;
        pieces[COLOR_BLACK][i] = 0;
    }

    //pieces[COLOR_WHITE][PAWN_BITBOARD_POSITION] = 0xff00;
    pieces[COLOR_WHITE][PAWN_BITBOARD_POSITION] = 0xff00;
    pieces[COLOR_BLACK][PAWN_BITBOARD_POSITION] = 0xff000000000000;

    pieces[COLOR_WHITE][ROOK_BITBOARD_POSITION] = 0x81;
    pieces[COLOR_WHITE][KNIGHT_BITBOARD_POSITION] = 0x42;
    pieces[COLOR_WHITE][BISHOP_BITBOARD_POSITION] = 0x24;
    pieces[COLOR_WHITE][QUEEN_BITBOARD_POSITION] = 0x10;
    pieces[COLOR_WHITE][KING_BITBOARD_POSITION] = 0x8;

    pieces[COLOR_BLACK][ROOK_BITBOARD_POSITION] = 0x8100000000000000;
    pieces[COLOR_BLACK][KNIGHT_BITBOARD_POSITION] = 0x4200000000000000;
    pieces[COLOR_BLACK][BISHOP_BITBOARD_POSITION] = 0x2400000000000000;
    pieces[COLOR_BLACK][QUEEN_BITBOARD_POSITION] = 0x1000000000000000;
    pieces[COLOR_BLACK][KING_BITBOARD_POSITION] = 0x800000000000000;
    //pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] = 0xff;

    //pieces[COLOR_BLACK][PAWN_BITBOARD_POSITION] = pieces[COLOR_WHITE][PAWN_BITBOARD_POSITION] << (8 * 5);
    //pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] = pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] << (8 * 6);
    turn = COLOR_WHITE;
    recalculate_masks();

    possible_plies.reserve(32);
}

Board::~Board()
{
}

Mask Board::white_push_targets(Mask pawns, Mask empty)
{
    return pawns.nortOne() & empty;
}
Mask Board::black_push_targets(Mask pawns, Mask empty)
{
    return pawns.soutOne() & empty;
}

Mask Board::white_double_push_targets(Mask single_push_targets, Mask empty)
{
    return single_push_targets.nortOne() & empty & RANK_4;
}
Mask Board::black_double_push_targets(Mask single_push_targets, Mask empty)
{
    return single_push_targets.soutOne() & empty & RANK_5;
}
//TODO enpasant and promotion
void Board::calculate_possible_plies_pawns(std::vector<Ply> &possible_plies, uint8_t color)
{
    Mask pawns = pieces[color][PAWN_BITBOARD_POSITION];
    Mask empty = ~occupied;
    //pawn pushes
    Mask single_push_targets;
    Mask double_push_targets;
    uint8_t offset = UINT8_MAX;
    if (color == COLOR_WHITE)
    {
        single_push_targets = white_push_targets(pawns, empty);
        double_push_targets = white_double_push_targets(single_push_targets, empty);
    }
    else
    {
        single_push_targets = black_push_targets(pawns, empty);
        double_push_targets = black_double_push_targets(single_push_targets, empty);
        offset = 1;
    }

    while (!single_push_targets.is_zero())
    {
        Location destination = single_push_targets.get_next_location();
        Location source = Location(destination.get_file(), destination.get_rank() + offset);
        possible_plies.push_back(Ply(Piece::Pawn, source, destination));
    }

    while (!double_push_targets.is_zero())
    {
        Location destination = double_push_targets.get_next_location();
        Location source = Location(destination.get_file(), (destination.get_rank() + offset) + offset);
        possible_plies.push_back(Ply(Piece::Pawn, source, destination));
    }
    //pawn attacks
    Mask enemy_occupied = pieces[other_color[color]][ALL_BITBOARD_POSITION];

    
    while (!pawns.is_zero())
    {
        Location location = pawns.get_next_location();
        Mask moves = table.get_pawn_attacks(color, enemy_occupied, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.push_back(Ply(Piece::Pawn, location, destination));
        }
    }
}
void Board::calculate_possible_plies_rook(std::vector<Ply> &possible_plies, uint8_t color)
{
    Mask rooks = pieces[color][ROOK_BITBOARD_POSITION];
    Mask own = pieces[color][ALL_BITBOARD_POSITION];
    while (!rooks.is_zero())
    {
        Location location = rooks.get_next_location();
        Mask moves = table.get_rook_attack(occupied, occupied_flipped, own, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.push_back(Ply(Piece::Rook, location, destination));
        }
    }
}
void Board::calculate_possible_plies_knight(std::vector<Ply> &possible_plies, uint8_t color)
{
    Mask knights = pieces[color][KNIGHT_BITBOARD_POSITION];
    Mask own = pieces[color][ALL_BITBOARD_POSITION];
    while (!knights.is_zero())
    {
        Location location = knights.get_next_location();
        Mask moves = table.get_knight_attack(own, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.push_back(Ply(Piece::Knight, location, destination));
        }
    }
}
void Board::calculate_possible_plies_bishop(std::vector<Ply> &possible_plies, uint8_t color)
{
    Mask bishops = pieces[color][BISHOP_BITBOARD_POSITION];
    Mask own = pieces[color][ALL_BITBOARD_POSITION];
    while (!bishops.is_zero())
    {
        Location location = bishops.get_next_location();
        Mask moves = table.get_bishop_attack(occupied_left, occupied_right, own, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.push_back(Ply(Piece::Bishop, location, destination));
        }
    }
}
void Board::calculate_possible_plies_queen(std::vector<Ply> &possible_plies, uint8_t color)
{
    Mask queens = pieces[color][QUEEN_BITBOARD_POSITION];
    Mask own = pieces[color][ALL_BITBOARD_POSITION];
    while (!queens.is_zero())
    {
        Location location = queens.get_next_location();
        Mask bishop_moves = table.get_bishop_attack(occupied_left, occupied_right, own, location);
        Mask rook_moves = table.get_rook_attack(occupied, occupied_flipped, own, location);
        while (!bishop_moves.is_zero())
        {
            Location destination = bishop_moves.get_next_location();
            possible_plies.push_back(Ply(Piece::Queen, location, destination));
        }
        while (!rook_moves.is_zero())
        {
            Location destination = rook_moves.get_next_location();
            possible_plies.push_back(Ply(Piece::Queen, location, destination));
        }
    }
}
//TODO castling
void Board::calculate_possible_plies_king(std::vector<Ply> &possible_plies, uint8_t color)
{
    Mask kings = pieces[color][KING_BITBOARD_POSITION];
    Mask own = pieces[color][ALL_BITBOARD_POSITION];
    while (!kings.is_zero())
    {
        Location location = kings.get_next_location();
        Mask moves = table.get_king_attack(own, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.push_back(Ply(Piece::King, location, destination));
        }
    }
}

void Board::recalculate_masks()
{
    pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] = 0;
    pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] = 0;

    for (uint8_t i = PAWN_BITBOARD_POSITION; i <= KING_BITBOARD_POSITION; i++)
    {
        pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] |= pieces[COLOR_BLACK][i];
        pieces[COLOR_WHITE][ALL_BITBOARD_POSITION] |= pieces[COLOR_WHITE][i];
    }

    occupied = pieces[COLOR_BLACK][ALL_BITBOARD_POSITION] | pieces[COLOR_WHITE][ALL_BITBOARD_POSITION];
    occupied_flipped = occupied.flip_anti_diag();

    occupied_left = occupied.rotate_left_45();
    occupied_right = occupied.rotate_right_45();
}

void Board::caluclate_possible_lies()
{
    possible_plies.clear();
    calculate_possible_plies_pawns(possible_plies, turn);
    calculate_possible_plies_rook(possible_plies, turn);
    calculate_possible_plies_knight(possible_plies, turn);
    calculate_possible_plies_bishop(possible_plies, turn);
    calculate_possible_plies_queen(possible_plies, turn);
    calculate_possible_plies_king(possible_plies, turn);

    /*
    for (Ply ply : possible_plies)
    {
        print_ply(ply);
    }
    */
    if (possible_plies.size() != 0)
    {
        Ply ply = possible_plies[rand() % possible_plies.size()];
        print_ply(ply);
        execute_ply(ply);
    }
    else
    {
        turn = other_color[turn];
    }
}

void Board::execute_ply(Ply ply)
{
    Location source = ply.get_source();
    Location destination = ply.get_destination();
    uint8_t mask_index = ply.get_piece_index();

    assert(pieces[turn][mask_index].is_set(source));
    assert(!pieces[turn][mask_index].is_set(destination));

    pieces[turn][mask_index].set(destination);
    pieces[turn][mask_index].reset(source);

    for (uint8_t i = PAWN_BITBOARD_POSITION; i <= KING_BITBOARD_POSITION; i++)
    {
        pieces[other_color[turn]][i].reset(destination);
    }

    recalculate_masks();

    turn = other_color[turn];

    std::cout << *this << std::endl;
}
//TODO This must not be fast but should output accurate Algebraic notation
void Board::print_ply(Ply ply)
{
    Location destination = ply.get_destination();
    Location source = ply.get_source();

    if (ply.get_piece() != Piece::Pawn)
    {
        std::cout << characters[static_cast<size_t>(ply.get_piece())];

        std::vector<Ply> plies_from_same_type;
        calculate_moves(plies_from_same_type, ply.get_piece(), turn);

        uint8_t same_file = 0;
        uint8_t same_rank = 0;

        for (Ply other_ply : plies_from_same_type)
        {
            if (other_ply.get_source() != source && other_ply.get_destination() == destination)
            {
                Location other_source = other_ply.get_source();

                if (source.get_file() == other_source.get_file())
                {
                    same_file++;
                }
                if (source.get_rank() == other_source.get_rank())
                {
                    same_rank++;
                }
            }
        }

        if (same_file != 0)
        {
            std::cout << source.get_file_char();

            if (same_rank != 0)
            {
                std::cout << source.get_rank_char();
            }
        }
        else if (same_rank != 0)
        {
            std::cout << source.get_rank_char();
        }
    }
    else
    {
        //Pawn
        if (occupied.is_set(destination))
        {
            std::cout << source.get_file_char();
        }
    }

    if (occupied.is_set(destination))
    {
        std::cout << 'x';
    }

    std::cout << destination << std::endl;
}

void Board::set(Location location, Piece piece, uint8_t color)
{
    for (uint8_t i = 0; i < NUMBER_BITBOARDS; i++)
    {
        pieces[other_color[color]][i].reset(location);
    }
    pieces[color][static_cast<uint8_t>(piece)].set(location);

    recalculate_masks();
}

//also slow but this is ok just dont use it in move generation
void Board::calculate_moves(std::vector<Ply> &possible_plies, Piece piece, uint8_t color)
{
    switch (piece)
    {
    case Piece::Pawn:
        calculate_possible_plies_pawns(possible_plies, color);
        break;
    case Piece::Rook:
        calculate_possible_plies_rook(possible_plies, color);
        break;
    case Piece::Knight:
        calculate_possible_plies_knight(possible_plies, color);
        break;
    case Piece::Bishop:
        calculate_possible_plies_bishop(possible_plies, color);
        break;
    case Piece::Queen:
        calculate_possible_plies_queen(possible_plies, color);
        break;
    case Piece::King:
        calculate_possible_plies_king(possible_plies, color);
        break;

    default:
        assert(false);
        break;
    }
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
    for (uint8_t y = 0; y != 8; y++)
    {
        board.print_line_position(y, os);
        board.print_line_pieces(y, os);
        board.print_line(y, os);
    }
    return os;
}
