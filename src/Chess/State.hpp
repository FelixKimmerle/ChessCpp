#include <cstdint>
#include <ostream>
#include <vector>
#include <array>
#include <string>
#include <stack>
#include <unordered_set>

#include "Mask.hpp"
#include "Ply.hpp"
#include "Color.hpp"
#include "LookupTable.hpp"
#include "StateInfo.hpp"

namespace std
{
    template <>
    struct hash<Ply>
    {
        size_t operator()(Ply value) const
        {
            return static_cast<size_t>((value.get_destination().get_square() << 10) | (value.get_source().get_square() << 4) | value.get_piece());
        }
    };
}

const static uint64_t NUMBER_BITBOARDS = 7;

class State
{
private:
    //Mask pieces[NUMBER_PLAYERS][NUMBER_BITBOARDS];
    //std::array<std::array<Mask, NUMBER_BITBOARDS>, NUMBER_PLAYERS> pieces;
    MultiDimArray<Mask, 2, NUMBER_BITBOARDS> pieces;
    Mask occupied;
    Mask occupied_flipped;
    Mask occupied_left;
    Mask occupied_right;
    Color turn : 2;
    std::unordered_set<Ply> possible_plies;
    const static LookupTable table;
    Mask enpasant;

    Mask white_push_targets(Mask pawns, Mask empty) const;
    Mask black_push_targets(Mask pawns, Mask empty) const;

    Mask white_double_push_targets(Mask single_push_targets, Mask empty) const;
    Mask black_double_push_targets(Mask single_push_targets, Mask empty) const;

    void calculate_possible_plies_pawns(std::unordered_set<Ply> &possible_plies, Color color) const;
    void calculate_possible_plies_rook(std::unordered_set<Ply> &possible_plies, Color color) const;
    void calculate_possible_plies_knight(std::unordered_set<Ply> &possible_plies, Color color) const;
    void calculate_possible_plies_bishop(std::unordered_set<Ply> &possible_plies, Color color) const;
    void calculate_possible_plies_queen(std::unordered_set<Ply> &possible_plies, Color color) const;
    void calculate_possible_plies_king(std::unordered_set<Ply> &possible_plies, Color color) const;

    void recalculate_masks();

    /*
        1. turn (0 - white, 1 - black)
        2. white is allowed long castling
        3. white is allowed short castling
        4. black is allowed long castling
        5. black is allowed short castling
        6 - 8 last move source
        9 - 11 last move destination
        12 - 15 pawn in case of promotion
    */
    char get_pawn_char(uint8_t color);
    void background(uint8_t x, uint8_t y, std::ostream &os) const;
    void print_line_pieces(uint8_t y, std::ostream &os) const;
    void print_piece(uint8_t y, uint8_t x, std::ostream &os) const;
    void print_line_position(uint8_t y, std::ostream &os) const;
    void print_line(uint8_t y, std::ostream &os) const;

    void calculate_moves(std::unordered_set<Ply> &possible_plies, Piece piece, Color color) const;

    StateInfo state_info;
    std::stack<StateInfo> info_history;

public:
    State(const std::string &fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    ~State();

    void caluclate_possible_plies();
    void execute_ply(Ply ply);
    void undo_ply(Ply ply);
    std::string get_algebraic_notation(Ply ply) const;
    void set(Location location, Piece piece, Color color = White);
    Piece get_piece(Location location, Color color) const;
    Color get_color(Location location) const;
    Color get_turn() const;
    bool is_possible(const Ply &ply) const;
    std::unordered_set<Ply> get_possible_plies()const;

    std::string to_fen() const;

    friend std::ostream &operator<<(std::ostream &os, const State &board);
};
