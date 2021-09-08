#include "MoveAnimation.hpp"

#include <iostream>

#include <cmath>
#define _USE_MATH_DEFINES

MoveAnimation::MoveAnimation(float speed, Ply ply, float tile_size, sf::Sprite sprite) : speed(speed), tile_size(tile_size), sprite(sprite), arg(0.f), ply(ply)
{
    start.x = ply.get_source().get_file() * tile_size + tile_size / 2;
    start.y = ply.get_source().get_rank() * tile_size + tile_size / 2;
    sf::Vector2f end(ply.get_destination().get_file() * tile_size + tile_size / 2, ply.get_destination().get_rank() * tile_size + tile_size / 2);
    dir = end - start;
    length = std::sqrt(dir.x * dir.x + dir.y * dir.y) / tile_size;
}

void MoveAnimation::update()
{
    arg = clock.getElapsedTime().asSeconds() * 0.5f * M_PI / speed;
    float ratio = std::sin(arg);
    if (ratio >= 1.f)
    {
        ratio = 1.f;
    }
    sprite.setPosition(start + dir * ratio);
}

void MoveAnimation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}

bool MoveAnimation::is_finished() const
{
    return arg >= 0.5f * M_PI;
}

uint8_t MoveAnimation::get_destination() const
{
    return ply.get_destination().get_square();
}