#include "State.hpp"
#include "ConsoleColor.hpp"
#include "Location.hpp"
#include "Color.hpp"

#include <bitset>
#include <iostream>
#include <cassert>
#include <sstream>
#include <cctype>

const LookupTable State::table;

enum BitboardLocation : uint8_t
{
    QUEEN_POSITION,
    KNIGHT_POSITION,
    BISHOP_POSITION,
    ROOK_POSITION,
    PAWN_POSITION,
    KING_POSITION,
    ALL_POSITION,
};

State::State(const std::string &fen)
{
    for (size_t i = 0; i < NUMBER_BITBOARDS; i++)
    {
        pieces[White][i] = 0;
        pieces[Black][i] = 0;
    }

    uint8_t location = 0;
    std::string::const_iterator it = fen.begin();

    while (location != 64)
    {
        char character = *it++;
        if (character == '/')
        {
            continue;
        }
        if (character >= '1' && character <= '8')
        {
            location += character - '0';
            continue;
        }
        uint8_t color = White;
        if (character >= 'a' && character <= 'z')
        {
            color = Black;
            character = (character - 'a') + 'A';
        }

        switch (character)
        {
        case 'Q':
            pieces[color][QUEEN_POSITION].set(location);
            break;
        case 'N':
            pieces[color][KNIGHT_POSITION].set(location);
            break;
        case 'B':
            pieces[color][BISHOP_POSITION].set(location);
            break;
        case 'R':
            pieces[color][ROOK_POSITION].set(location);
            break;
        case 'P':
            pieces[color][PAWN_POSITION].set(location);
            break;
        case 'K':
            pieces[color][KING_POSITION].set(location);
            break;
        }
        location++;
    }

    it++;
    if (*it == 'w')
    {
        turn = White;
    }
    else if (*it == 'b')
    {
        turn = Black;
    }

    for (size_t i = 0; i < NUMBER_BITBOARDS; i++)
    {
        pieces[White][i] = pieces[White][i].flip_vertical();
        pieces[Black][i] = pieces[Black][i].flip_vertical();
    }

    recalculate_masks();

    possible_plies.reserve(32);

    caluclate_possible_plies();
}

State::~State()
{
}

Mask State::white_push_targets(Mask pawns, Mask empty) const
{
    return pawns.nortOne() & empty;
}
Mask State::black_push_targets(Mask pawns, Mask empty) const
{
    return pawns.soutOne() & empty;
}

Mask State::white_double_push_targets(Mask single_push_targets, Mask empty) const
{
    return single_push_targets.nortOne() & empty & RANK_4;
}
Mask State::black_double_push_targets(Mask single_push_targets, Mask empty) const
{
    return single_push_targets.soutOne() & empty & RANK_5;
}
// TODO enpasant and promotion
void State::calculate_possible_plies_pawns(std::unordered_set<Ply> &possible_plies, Color color) const
{
    Mask pawns = pieces[color][PAWN_POSITION];
    Mask empty = ~occupied;
    // pawn pushes
    Mask single_push_targets;
    Mask double_push_targets;
    uint8_t offset = UINT8_MAX; // = -1
    uint8_t promotion_rank = color == Color::White ? 7 : 0;

    if (color == White)
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
        if (destination.get_rank() != promotion_rank)
        {
            possible_plies.insert(Ply(Piece::Pawn, source, destination));
        }
        else
        {
            possible_plies.insert(Ply(Piece::Pawn, source, destination, Piece::Queen));
            possible_plies.insert(Ply(Piece::Pawn, source, destination, Piece::Knight));
            possible_plies.insert(Ply(Piece::Pawn, source, destination, Piece::Bishop));
            possible_plies.insert(Ply(Piece::Pawn, source, destination, Piece::Rook));
        }
    }

    while (!double_push_targets.is_zero())
    {
        Location destination = double_push_targets.get_next_location();
        Location source = Location(destination.get_file(), (destination.get_rank() + offset) + offset);
        possible_plies.insert(Ply(Piece::Pawn, source, destination));
    }
    // pawn attacks
    Mask enemy_occupied = pieces[other_color(color)][ALL_POSITION];

    while (!pawns.is_zero())
    {
        Location location = pawns.get_next_location();
        Mask moves = table.get_pawn_attacks(color, enemy_occupied, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            if (destination.get_rank() != promotion_rank)
            {
                possible_plies.insert(Ply(Piece::Pawn, location, destination));
            }
            else
            {
                possible_plies.insert(Ply(Piece::Pawn, location, destination, Piece::Queen));
                possible_plies.insert(Ply(Piece::Pawn, location, destination, Piece::Knight));
                possible_plies.insert(Ply(Piece::Pawn, location, destination, Piece::Bishop));
                possible_plies.insert(Ply(Piece::Pawn, location, destination, Piece::Rook));
            }
        }
    }
}
void State::calculate_possible_plies_rook(std::unordered_set<Ply> &possible_plies, Color color) const
{
    Mask rooks = pieces[color][ROOK_POSITION];
    Mask own = pieces[color][ALL_POSITION];
    while (!rooks.is_zero())
    {
        Location location = rooks.get_next_location();
        Mask moves = table.get_rook_attack(occupied, occupied_flipped, own, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.insert(Ply(Piece::Rook, location, destination));
        }
    }
}
void State::calculate_possible_plies_knight(std::unordered_set<Ply> &possible_plies, Color color) const
{
    Mask knights = pieces[color][KNIGHT_POSITION];
    Mask own = pieces[color][ALL_POSITION];
    while (!knights.is_zero())
    {
        Location location = knights.get_next_location();
        Mask moves = table.get_knight_attack(own, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.insert(Ply(Piece::Knight, location, destination));
        }
    }
}
void State::calculate_possible_plies_bishop(std::unordered_set<Ply> &possible_plies, Color color) const
{
    Mask bishops = pieces[color][BISHOP_POSITION];
    Mask own = pieces[color][ALL_POSITION];
    while (!bishops.is_zero())
    {
        Location location = bishops.get_next_location();
        Mask moves = table.get_bishop_attack(occupied_left, occupied_right, own, location);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.insert(Ply(Piece::Bishop, location, destination));
        }
    }
}
void State::calculate_possible_plies_queen(std::unordered_set<Ply> &possible_plies, Color color) const
{
    Mask queens = pieces[color][QUEEN_POSITION];
    Mask own = pieces[color][ALL_POSITION];

    Mask thread = 0;
    while (!queens.is_zero())
    {
        Location location = queens.get_next_location();
        Mask bishop_moves = table.get_bishop_attack(occupied_left, occupied_right, own, location);
        Mask rook_moves = table.get_rook_attack(occupied, occupied_flipped, own, location);
        while (!bishop_moves.is_zero())
        {
            Location destination = bishop_moves.get_next_location();
            possible_plies.insert(Ply(Piece::Queen, location, destination));
        }
        while (!rook_moves.is_zero())
        {
            Location destination = rook_moves.get_next_location();
            possible_plies.insert(Ply(Piece::Queen, location, destination));
        }
    }
}
// TODO castling
void State::calculate_possible_plies_king(std::unordered_set<Ply> &possible_plies, Color color, Mask attack_mask) const
{
    Mask kings = pieces[color][KING_POSITION];
    Mask own = pieces[color][ALL_POSITION];
    while (!kings.is_zero())
    {
        Location location = kings.get_next_location();
        Mask moves = table.get_king_attack(own, location) & (~attack_mask);
        while (!moves.is_zero())
        {
            Location destination = moves.get_next_location();
            possible_plies.insert(Ply(Piece::King, location, destination));
        }
    }
}

void State::recalculate_masks()
{
    pieces[Black][ALL_POSITION] = 0;
    pieces[White][ALL_POSITION] = 0;

    for (uint8_t i = 0; i < ALL_POSITION; i++)
    {
        pieces[Black][ALL_POSITION] |= pieces[Black][i];
        pieces[White][ALL_POSITION] |= pieces[White][i];
    }

    occupied = pieces[Black][ALL_POSITION] | pieces[White][ALL_POSITION];
    occupied_flipped = occupied.flip_anti_diag();

    occupied_left = occupied.rotate_left_45();
    occupied_right = occupied.rotate_right_45();

    opponent_attack_mask = calculate_attack_mask(other_color(turn));
}

Mask State::calculate_pinned_pieces(Color player)
{
    Mask pinned_pieces;
    Mask kings = pieces[player][QUEEN_POSITION];
    while (!kings.is_zero())
    {
        Location location = kings.get_next_location();
        Mask rank = ranks[location.get_rank()];
        Mask file = files[location.get_file()];

        
        //Mask bishop_moves = table.get_bishop_attack(occupied_left, occupied_right, 0, location);
        //Mask rook_moves = table.get_rook_attack(occupied, occupied_flipped, 0, location);
        // king_rays |= bishop_moves;
        // king_rays |= rook_moves;

        // std::cout << "diag: " << (int)diag << std::endl;
        // std::cout << "anti diag: " << (int)anti_diag << std::endl;
        // Color opponent_color = other_color(turn);
        // Mask opponent_diag_sliders = pieces[opponent_color][QUEEN_POSITION] | pieces[opponent_color][BISHOP_POSITION];
        // Mask opponent_straight_sliders = pieces[opponent_color][QUEEN_POSITION] | pieces[opponent_color][ROOK_POSITION];
        // // pieces[opponent_color][QUEEN_POSITION].get_line
        // Mask opponent_straight_sliders_flittped = opponent_straight_sliders.flip_anti_diag();
        // Mask own = pieces[turn][ALL_POSITION];
        // Mask own_flipped = pieces[turn][ALL_POSITION].flip_anti_diag();

        // uint8_t file = location.get_file();
        // Line occupied_file = opponent_straight_sliders_flittped.get_line(file);

        // uint8_t rank = position.get_rank();
        // Line occupied_rank = occupied.get_line(rank);

        // Mask occupied_left_opponent = pieces[other_color(turn)][ALL_POSITION].rotate_left_45();
        // Mask occupied_right_opponent = occupied.rotate_right_45();

        // uint8_t diag = location.get_diagonal();
        // uint8_t anti_diag = location.get_anti_diagonal();

        // Line diag_occupied_opponent = occupied_left_opponent.get_diagonal(diag);
        // Line anti_diag_occupied = occupied_right_opponent.get_anti_diagonal(anti_diag);

        // std::cout << "diag_occupied_opponent: " << std::endl;
        // std::cout << diag_occupied_opponent << std::endl;
    }

    return pinned_pieces;
}

bool State::is_check() const
{
    return (opponent_attack_mask & pieces[turn][KING_POSITION]) != 0;
}


bool State::is_check_mate() const
{
    std::unordered_set<Ply> possible_king_plies;
    calculate_possible_plies_king(possible_king_plies, turn, opponent_attack_mask);
    std::cout << possible_king_plies.size() << std::endl;
    for(const auto & ply : possible_king_plies)
    {
        std::cout << get_algebraic_notation(ply) << std::endl;
    }
    return possible_king_plies.size() == 0;
}

Mask State::calculate_attack_mask(Color player)
{
    Mask occupied = 0;

    for (uint8_t i = 0; i <= PAWN_POSITION; i++)
    {
        occupied |= pieces[Black][i] | pieces[White][i];
    }

    Mask occupied_flipped = occupied.flip_anti_diag();

    Mask occupied_left = occupied.rotate_left_45();
    Mask occupied_right = occupied.rotate_right_45();


    Mask attack_mask;
    {
        Mask pawns = pieces[player][PAWN_POSITION];
        while (!pawns.is_zero())
        {
            Location location = pawns.get_next_location();
            Mask moves = table.get_pawn_attacks(player, location);
            attack_mask |= moves;
        }
    }

    {
        Mask rooks = pieces[player][ROOK_POSITION];
        while (!rooks.is_zero())
        {
            Location location = rooks.get_next_location();
            Mask moves = table.get_rook_attack(occupied, occupied_flipped, 0, location);
            attack_mask |= moves;
        }
    }
    {
        Mask knights = pieces[player][KNIGHT_POSITION];
        while (!knights.is_zero())
        {
            Location location = knights.get_next_location();
            Mask moves = table.get_knight_attack(0, location);
            attack_mask |= moves;
        }
    }
    {
        Mask bishops = pieces[player][BISHOP_POSITION];
        while (!bishops.is_zero())
        {
            Location location = bishops.get_next_location();
            Mask moves = table.get_bishop_attack(occupied_left, occupied_right, 0, location);
            attack_mask |= moves;
        }
    }

    {

        Mask queens = pieces[player][QUEEN_POSITION];
        while (!queens.is_zero())
        {
            Location location = queens.get_next_location();
            Mask bishop_moves = table.get_bishop_attack(occupied_left, occupied_right, 0, location);
            Mask rook_moves = table.get_rook_attack(occupied, occupied_flipped, 0, location);
            attack_mask |= bishop_moves;
            attack_mask |= rook_moves;
        }
    }
    {

        Mask kings = pieces[player][KING_POSITION];
        while (!kings.is_zero())
        {
            Location location = kings.get_next_location();
            Mask moves = table.get_king_attack(0, location);
            attack_mask |= moves;
        }
    }
    return attack_mask;
}

void State::caluclate_possible_plies()
{
    // std::cout << to_char(other_color(turn)) << std::endl;
    // std::cout << opponent_attack_mask << std::endl;
    // std::cout << to_char(turn) << std::endl;
    // std::cout << calculate_attack_mask(turn) << std::endl;
    possible_plies.clear();
    calculate_possible_plies_pawns(possible_plies, turn);
    calculate_possible_plies_rook(possible_plies, turn);
    calculate_possible_plies_knight(possible_plies, turn);
    calculate_possible_plies_bishop(possible_plies, turn);
    calculate_possible_plies_queen(possible_plies, turn);
    calculate_possible_plies_king(possible_plies, turn, opponent_attack_mask);

    /*
    for (Ply ply : possible_plies)
    {
        std::cout << get_algebraic_notation(ply) << std::endl;
    }
    */
    /*
    if (possible_plies.size() != 0)
    {
        Ply ply = possible_plies[rand() % possible_plies.size()];
        get_algebraic_notation(ply);
        execute_ply(ply);
    }
    else
    {
        turn = other_color(turn);
    }
    */
}

void State::execute_ply(Ply ply)
{
    Location source = ply.get_source();
    Location destination = ply.get_destination();
    uint8_t mask_index_source = static_cast<uint8_t>(ply.get_piece());
    uint8_t mask_index_destination = mask_index_source;
    if (ply.is_promotion())
    {
        mask_index_destination = static_cast<uint8_t>(ply.get_ply_type());
    }

    assert(pieces[turn][mask_index_source].is_set(source));
    assert(!pieces[turn][mask_index_source].is_set(destination));

    pieces[turn][mask_index_destination].set(destination);
    pieces[turn][mask_index_source].reset(source);

    Piece changed = Empty;
    for (uint8_t i = 0; i < ALL_POSITION; i++)
    {
        if (pieces[other_color(turn)][i].is_set(destination))
        {
            changed = static_cast<Piece>(i);
        }
        pieces[other_color(turn)][i].reset(destination);
    }
    StateInfo state_info;
    state_info.set_captured_piece(changed);
    info_history.push(state_info);

    turn = other_color(turn);
    recalculate_masks();

    // std::cout << *this << std::endl;

    caluclate_possible_plies();
}

void State::undo_ply(Ply ply)
{
    Ply reverse(ply.get_piece(), ply.get_destination(), ply.get_source());
    turn = other_color(turn);

    Location source = reverse.get_source();
    Location destination = reverse.get_destination();
    uint8_t mask_index_destinaton = static_cast<uint8_t>(reverse.get_piece());
    uint8_t mask_index_source = mask_index_destinaton;
    if (ply.is_promotion())
    {
        mask_index_source = static_cast<uint8_t>(ply.get_ply_type());
    }

    assert(pieces[turn][mask_index_source].is_set(source));
    assert(!pieces[turn][mask_index_destinaton].is_set(destination));

    pieces[turn][mask_index_source].reset(source);
    pieces[turn][mask_index_destinaton].set(destination);

    for (uint8_t i = 0; i < ALL_POSITION; i++)
    {
        pieces[other_color(turn)][i].reset(destination);
    }

    StateInfo state_info = info_history.top();
    info_history.pop();
    Piece captured_piece = state_info.get_captured_piece();
    if (captured_piece != Empty)
    {
        pieces[other_color(turn)][captured_piece].set(source);
    }

    recalculate_masks();
    caluclate_possible_plies();
}

// TODO This must not be fast but should output accurate Algebraic notation
std::string State::get_algebraic_notation(Ply ply) const
{
    std::stringstream ss;
    Location destination = ply.get_destination();
    Location source = ply.get_source();

    if (ply.get_piece() != Piece::Pawn)
    {
        ss << characters[static_cast<size_t>(ply.get_piece())];

        std::unordered_set<Ply> plies_from_same_type;
        calculate_moves(plies_from_same_type, ply.get_piece(), turn);

        bool same_file = false;
        bool same_rank = false;
        bool same_dest = false;

        for (Ply other_ply : plies_from_same_type)
        {
            if (other_ply.get_source() != source && other_ply.get_destination() == destination)
            {
                Location other_source = other_ply.get_source();
                same_dest = true;
                if (source.get_file() == other_source.get_file())
                {
                    same_file = true;
                }
                if (source.get_rank() == other_source.get_rank())
                {
                    same_rank = true;
                }
            }
        }

        if (same_dest)
        {
            if (!(same_file && !same_rank))
            {
                ss << source.get_file_char();
            }

            if (same_file)
            {
                ss << source.get_rank_char();
            }
        }
    }
    else
    {
        // Pawn
        if (occupied.is_set(destination))
        {
            ss << source.get_file_char();
        }
    }

    if (occupied.is_set(destination))
    {
        ss << 'x';
    }

    ss << destination;
    State copy = *this;
    copy.execute_ply(ply);
    if(copy.is_check())
    {
        ss << "+";
    }

    return ss.str();
}

void State::set(Location location, Piece piece, Color color)
{
    for (uint8_t i = 0; i < NUMBER_BITBOARDS; i++)
    {
        pieces[other_color(color)][i].reset(location);
        pieces[color][i].reset(location);
    }

    pieces[color][piece].set(location);

    recalculate_masks();
    caluclate_possible_plies();
}

std::string State::to_fen() const
{
    std::stringstream ss;

    for (int8_t rank = 7; rank >= 0; rank--)
    {
        uint8_t empty = 0;
        for (uint8_t file = 0; file < 8; file++)
        {

            Location location(file, rank);
            Mask mask = location.get_mask();
            Color color = get_color(location);

            if (color != NoColor)
            {
                if (empty != 0)
                {
                    ss << static_cast<int>(empty);
                    empty = 0;
                }

                Piece piece = get_piece(location, color);
                char piece_char = characters[piece];
                if (color == Black)
                {
                    piece_char = piece_char - 'A' + 'a';
                }

                ss << piece_char;
            }
            else
            {
                empty++;
            }
        }
        if (empty != 0)
        {
            ss << static_cast<int>(empty);
        }
        if (rank != 0)
        {
            ss << "/";
        }
    }

    ss << " " << to_char(turn) << " ";

    return ss.str();
}

void State::reset()
{
    while (!info_history.empty())
    {
        info_history.pop();
    }

    for (uint8_t i = 0; i <= ALL_POSITION; i++)
    {
        pieces[Color::White][i] = 0;
        pieces[Color::Black][i] = 0;
    }

    occupied = 0;
    occupied_flipped = 0;
    occupied_left = 0;
    occupied_right = 0;
    turn = Color::White;
    enpasant = 0;
    possible_plies.clear();
}

void State::reset_start()
{
    //reset();
    *this = State();
}

Mask State::get_mask(Color color, uint8_t bitmap) const
{
    return pieces[color][bitmap];
}

// also slow but this is ok just dont use it in move generation
void State::calculate_moves(std::unordered_set<Ply> &possible_plies, Piece piece, Color color) const
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
        calculate_possible_plies_king(possible_plies, color, Mask());
        break;

    default:
        assert(false);
        break;
    }
}

Piece State::get_piece(Location location, Color color) const
{
    Mask mask = location.get_mask();
    if ((pieces[color][ALL_POSITION] & mask) == 0)
    {
        return Piece::Empty;
    }

    uint8_t counter = 0;

    while ((pieces[color][counter] & mask) == 0)
    {
        counter++;
    }

    assert(counter <= static_cast<uint8_t>(Piece::Empty));

    return static_cast<Piece>(counter);
}

Color State::get_color(Location location) const
{
    Mask mask = location.get_mask();

    if ((pieces[Black][ALL_POSITION] & mask) != 0)
    {
        return Black;
    }
    else if ((pieces[White][ALL_POSITION] & mask) != 0)
    {
        return White;
    }
    return NoColor;
}

Color State::get_turn() const
{
    return turn;
}

bool State::is_possible(const Ply &ply) const
{
    return possible_plies.count(ply) >= 1;
}

std::unordered_set<Ply> State::get_possible_plies() const
{
    return possible_plies;
}

void State::background(uint8_t x, uint8_t y, std::ostream &os) const
{
    if ((x + y % 2) % 2 == 0)
    {
        os << ConsoleColor::Backgound::Grey;
    }
    else
    {
        os << ConsoleColor::Backgound::Lime;
    }
}

void State::print_line(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << "       " << ConsoleColor::Reset;
    }
    os << std::endl;
}

void State::print_piece(uint8_t x, uint8_t y, std::ostream &os) const
{
    Location location(x, y);
    Mask mask = location.get_mask();
    Color color = get_color(location);
    if (color != NoColor)
    {
        if (color == White)
        {
            os << ConsoleColor::Text::White;
        }
        else if (color == Black)
        {
            os << ConsoleColor::Text::Black;
        }
        os << characters[get_piece(location, color)];
    }
    else
    {
        os << " ";
    }
}

void State::print_line_pieces(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << "   ";
        print_piece(x, y, os);
        os << "   " << ConsoleColor::Reset;
    }
    os << std::endl;
}

void State::print_line_position(uint8_t y, std::ostream &os) const
{
    for (uint8_t x = 0; x < 8; x++)
    {
        background(x, y, os);
        os << ConsoleColor::Text::Red << Location(x, y);
        os << "     " << ConsoleColor::Reset;
    }
    os << std::endl;
}
std::ostream &operator<<(std::ostream &os, const State &board)
{
    for (uint8_t y = 0; y != 8; y++)
    {
        board.print_line_position(y, os);
        board.print_line_pieces(y, os);
        board.print_line(y, os);
    }
    return os;
}
