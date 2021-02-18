#pragma once

#include <SFML/Graphics.hpp>
#include "Chess/Ply.hpp"

class MoveAnimation : public sf::Drawable
{
private:
    sf::Clock clock;
    sf::Sprite sprite;
    float speed;
    float tile_size;
    float length;
    sf::Vector2f dir;
    sf::Vector2f start;
    float arg;
    Ply ply;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    MoveAnimation(float speed, Ply ply, float tile_size, sf::Sprite sprite);
    void update();
    bool is_finished()const;
    uint8_t get_destination()const;
};



