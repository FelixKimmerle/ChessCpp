#pragma once
#include <cstdint>
#include <ostream>

class Line
{
private:
    uint8_t data;

    void background(uint8_t x, std::ostream &os)const;
    void print_line(std::ostream &os)const;
    void print_line_values(std::ostream &os)const;
    void print_line_position(std::ostream &os)const;
public:
    Line(uint8_t data);
    Line();
    ~Line();

    uint8_t get_raw()const;
    bool is_set(uint8_t bit_position)const;
    void set(uint8_t bit_position);
    Line operator|(const Line &other)const;
    Line operator|=(const Line &other);
    Line operator&(const Line &other)const;
    Line operator&=(const Line &other);
    Line operator^(const Line &other)const;
    Line operator^=(const Line &other);
    Line operator>>(const Line &other)const;
    Line operator>>=(const Line &other);
    Line operator<<(const Line &other)const;
    Line operator<<=(const Line &other);
    Line reverse()const;
    friend std::ostream &operator<<(std::ostream &os, const Line &line);

};

