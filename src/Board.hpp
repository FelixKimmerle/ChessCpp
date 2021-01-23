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
    std::array<std::array<Mask, NUMBER_BITBOARDS>, NUMBER_PLAYERS> pieces;
    uint16_t data;
    std::vector<Ply> possible_plies;
    LookupTable table;

    void calculate_possible_plies_pawns();
    void calculate_possible_plies_rook();
    void calculate_possible_plies_knight();
    void calculate_possible_plies_bishop();
    void calculate_possible_plies_queen();
    void calculate_possible_plies_king();

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

public:
    Board(/* args */);
    ~Board();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
