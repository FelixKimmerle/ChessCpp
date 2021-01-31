#include <iostream>
#include "Board.hpp"
#include "Line.hpp"
#include <iomanip>
#include "LookupTable.hpp"

int main()
{
    Board board;
    //board.set(Location("d4"),Piece::Queen,COLOR_WHITE);
    //board.set(Location("e4"),Piece::Queen,COLOR_WHITE);
    //board.set(Location("d5"),Piece::Queen,COLOR_WHITE);

    for (size_t i = 0; i < 300; i++)
    {
        board.caluclate_possible_lies();
    }

    //board.execute_ply(Ply(Piece::Pawn, Location(3, 1), Location(3, 3)));
    //board.caluclate_possible_lies();

    //std::cout << board << std::endl;
    //std::cout << Mask((CENTER_BIG.get_raw() | LONG_DIAGONALS.get_raw()) ^ CENTER_SQUARES.get_raw()) << std::endl;

    std::cout << board << std::endl;
    std::cout << sizeof(Ply) << std::endl;
    std::cout << sizeof(Mask) << std::endl;
    std::cout << sizeof(Location) << std::endl;
    std::cout << sizeof(LookupTable) << std::endl;

    /*

    Mask test = Mask(0x2222121e22221e00);
    std::cout << test << std::endl;
    std::cout << test.flip_vertical() << std::endl;
    std::cout << test.flip_main_diag() << std::endl;
    std::cout << test.flip_anti_diag() << std::endl;
    std::cout << test.rotate_right_90() << std::endl;
    std::cout << test.rotate_left_90() << std::endl;
    
    

    Mask test = Mask(0x1224489220418204).flip_horizontal();

    //Mask test(0x1224489020418204);
    //Mask test(0x22120a0e1222221e);
    std::cout << test << std::endl;
    std::cout << test.rotate_right_45() << std::endl;
    std::cout << test.rotate_left_45() << std::endl;

    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 8; x++)
        {
            std::cout << Location(x, y) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 8; x++)
        {
            std::cout << std::setw(2) << std::setfill('0') << (int)Location(x, y).get_diagonal() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 8; x++)
        {
            std::cout << std::setw(2) << std::setfill('0') << (int)Location(x, y).get_anti_diagonal() << " ";
        }
        std::cout << std::endl;
    }

    for (size_t i = 0; i < 15; i++)
    {
        std::cout << test.rotate_right_45().get_anti_diagonal(i);
    }
    std::cout << std::endl;
    for (size_t i = 0; i < 15; i++)
    {
        std::cout << test.rotate_left_45().get_diagonal(i);
    }
    */
}