#include <iostream>
#include "Board.hpp"
#include "Line.hpp"
#include <iomanip>
#include "LookupTable.hpp"

int main()
{
    Board board;
    //std::cout << board << std::endl;
    //std::cout << Mask((CENTER_BIG.get_raw() | LONG_DIAGONALS.get_raw()) ^ CENTER_SQUARES.get_raw()) << std::endl;

    LookupTable table;

    std::cout << sizeof(Ply) << std::endl;
    std::cout << sizeof(Mask) << std::endl;
    std::cout << sizeof(Location) << std::endl;
    std::cout << sizeof(LookupTable) << std::endl;

    
    /*
    Mask test(0x1224489020418204);
    //Mask test(0x22120a0e1222221e);
    std::cout << test << std::endl;
    std::cout << test.rotate_right_45() << std::endl;
    std::cout << test.rotate_left_45() << std::endl;

    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 8; x++)
        {
            std::cout << Location(x, 7 - y) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 8; x++)
        {
            std::cout << std::setw(2) << std::setfill('0') << (int)Location(x, 7 - y).get_anti_diagonal() << " ";
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