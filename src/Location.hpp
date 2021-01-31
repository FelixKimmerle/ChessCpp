#pragma once
#include <cstdint>
#include <ostream>
class Mask;
class Location
{
private:
    uint8_t x: 3;
    uint8_t y: 3;
public:
    Location(uint8_t number);
    Location(uint8_t x, uint8_t y);
    Location(const char name[2]);
    ~Location();

    Mask get_mask()const;
    uint8_t get_file()const;
    uint8_t get_rank()const;
    uint8_t get_file_flipped()const;
    uint8_t get_rank_flipped()const;
    uint8_t get_number()const;
    char get_file_char()const;
    char get_rank_char()const;
    uint8_t get_diagonal()const;
    uint8_t get_anti_diagonal()const;

    bool operator==(const Location &other)const;
    bool operator!=(const Location &other)const;

    friend std::ostream &operator<<(std::ostream &os, const Location &location);
};




/*
Location location_from(uint8_t x, uint8_t y);
uint8_t get_x(Location location);
uint8_t get_y(Location location);
Mask get_mask(Location position);
Mask get_mask(uint8_t x, uint8_t y);
char file_char(uint8_t x);
char rank_char(uint8_t y);
char file_char_location(Location location);
char rank_char_location(Location location);
void print_mask(uint64_t mask);
*/