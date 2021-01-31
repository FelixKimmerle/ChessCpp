#include <cstdint>
#include <ostream>
#include <vector>
#include <array>

#include "Definitions.hpp"
#include "Mask.hpp"
#include "Ply.hpp"
#include "LookupTable.hpp"

class Board
{
private:
    //Mask pieces[NUMBER_PLAYERS][NUMBER_BITBOARDS];
    //std::array<std::array<Mask, NUMBER_BITBOARDS>, NUMBER_PLAYERS> pieces;
    MultiDimArray<Mask, NUMBER_PLAYERS, NUMBER_BITBOARDS> pieces;
    Mask occupied;
    Mask occupied_flipped;
    Mask occupied_left;
    Mask occupied_right;
    uint8_t turn : 1;
    std::vector<Ply> possible_plies;
    static LookupTable table;
    Mask enpasant;

    Mask white_push_targets(Mask pawns, Mask empty);
    Mask black_push_targets(Mask pawns, Mask empty);

    Mask white_double_push_targets(Mask single_push_targets, Mask empty);
    Mask black_double_push_targets(Mask single_push_targets, Mask empty);

    void calculate_possible_plies_pawns(std::vector<Ply> &possible_plies, uint8_t color);
    void calculate_possible_plies_rook(std::vector<Ply> &possible_plies, uint8_t color);
    void calculate_possible_plies_knight(std::vector<Ply> &possible_plies, uint8_t color);
    void calculate_possible_plies_bishop(std::vector<Ply> &possible_plies, uint8_t color);
    void calculate_possible_plies_queen(std::vector<Ply> &possible_plies, uint8_t color);
    void calculate_possible_plies_king(std::vector<Ply> &possible_plies, uint8_t color);

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

    void calculate_moves(std::vector<Ply> &possible_plies, Piece piece, uint8_t color);

public:
    Board(/* args */);
    ~Board();

    void caluclate_possible_lies();
    void execute_ply(Ply ply);
    void print_ply(Ply ply);
    void set(Location location, Piece piece, uint8_t color);

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
