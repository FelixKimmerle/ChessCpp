#include "ChessWidget.hpp"

#include "imgui/imgui.h"

static const std::string piece_names[] = {"queen", "knight", "bishop", "rook", "pawn", "king"};

ChessWidget::ChessWidget(float size) : light_square(sf::Quads, 4),
                                       dark_square(sf::Quads, 4),
                                       light_square_color{165.f / 255.f, 106.f / 255.f, 23.f / 255.f},
                                       dark_square_color{86.f / 255.f, 38.f / 255.f, 20.f / 255.f},
                                       state("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"),
                                       selected_piece(Empty)
{
    sf::Color light_color(light_square_color[0] * 255, light_square_color[1] * 255, light_square_color[2] * 255);
    sf::Color dark_color(dark_square_color[0] * 255, dark_square_color[1] * 255, dark_square_color[2] * 255);

    light_square[0].color = light_color;
    light_square[1].color = light_color;
    light_square[2].color = light_color;
    light_square[3].color = light_color;

    light_square[0].texCoords = sf::Vector2f(0, 0);
    light_square[1].texCoords = sf::Vector2f(128, 0);
    light_square[2].texCoords = sf::Vector2f(128, 128);
    light_square[3].texCoords = sf::Vector2f(0, 128);

    dark_square[0].color = dark_color;
    dark_square[1].color = dark_color;
    dark_square[2].color = dark_color;
    dark_square[3].color = dark_color;

    dark_square[0].texCoords = sf::Vector2f(0, 0);
    dark_square[1].texCoords = sf::Vector2f(128, 0);
    dark_square[2].texCoords = sf::Vector2f(128, 128);
    dark_square[3].texCoords = sf::Vector2f(0, 128);

    for (size_t i = 0; i < 6; i++)
    {
        textures[i * 2].loadFromFile("res/128px/w_" + piece_names[i] + "_png_shadow_128px.png");
        textures[i * 2].setSmooth(true);
        textures[i * 2 + 1].loadFromFile("res/128px/b_" + piece_names[i] + "_png_shadow_128px.png");
        textures[i * 2 + 1].setSmooth(true);
    }

    resize(size);
}

ChessWidget::~ChessWidget()
{
}

void ChessWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::RenderStates orig_state = states;

    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            states.transform.translate(sf::Vector2f(tile_size * x, tile_size * y));
            if ((x + y % 2) % 2 == 0)
            {
                target.draw(light_square, states);
            }
            else
            {
                target.draw(dark_square, states);
            }
            states = orig_state;
        }
    }

    for (auto &&sprite : sprites)
    {
        target.draw(sprite, states);
    }

    if (selected_location.is_valid())
    {
        target.draw(selected_sprite, states);
    }

    for (auto &&animation : animations)
    {
        target.draw(animation, states);
    }
}

void ChessWidget::resize(float size)
{
    tile_size = size / 8.0f;
    light_square[0].position = sf::Vector2f(0, 0);
    light_square[1].position = sf::Vector2f(tile_size, 0);
    light_square[2].position = sf::Vector2f(tile_size, tile_size);
    light_square[3].position = sf::Vector2f(0, tile_size);

    dark_square[0].position = sf::Vector2f(0, 0);
    dark_square[1].position = sf::Vector2f(tile_size, 0);
    dark_square[2].position = sf::Vector2f(tile_size, tile_size);
    dark_square[3].position = sf::Vector2f(0, tile_size);

    reload();
}

void ChessWidget::draw_gui()
{
    if (ImGui::ColorEdit3("Light color", light_square_color))
    {
        sf::Color light_color(light_square_color[0] * 255, light_square_color[1] * 255, light_square_color[2] * 255);

        light_square[0].color = light_color;
        light_square[1].color = light_color;
        light_square[2].color = light_color;
        light_square[3].color = light_color;
    }
    if (ImGui::ColorEdit3("Dark color", dark_square_color))
    {
        sf::Color dark_color(dark_square_color[0] * 255, dark_square_color[1] * 255, dark_square_color[2] * 255);

        dark_square[0].color = dark_color;
        dark_square[1].color = dark_color;
        dark_square[2].color = dark_color;
        dark_square[3].color = dark_color;
    }
    ImGui::NewLine();
    for (auto it = moves.begin(); it != moves.end(); it++)
    {
        ImGui::Text(it->c_str());
        if (it + 1 != moves.end())
        {
            ImGui::SameLine();
            it++;
            std::string line = "\t" + *it;
            ImGui::Text(line.c_str());
        }
    }
}

void ChessWidget::update()
{
    for (auto &&animation : animations)
    {
        animation.update();
    }
    bool removed = false;
    for (int i = 0; i < animations.size(); i++)
    {
        if (animations[i].is_finished())
        {
            animated.erase(animations[i].get_destination());
            animations.erase(animations.begin() + i);
            i--;
            removed = true;
        }
    }
    if (removed)
    {
        reload();
    }
}

void ChessWidget::undo()
{
    if (history.size() != 0)
    {
        Ply ply = history.top();
        history.pop();
        state.undo_ply(ply);
        animated.insert(ply.get_source().get_square());
        reload();
        Ply reverse(ply.get_piece(), ply.get_destination(), ply.get_source());

        sf::Sprite sprite(textures[2 * ply.get_piece() + state.get_turn()]);
        sprite.scale(tile_size / sprite.getTexture()->getSize().x * 0.7f, tile_size / sprite.getTexture()->getSize().y * 0.7f);
        sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);

        animations.push_back(MoveAnimation(0.5f, reverse, tile_size, sprite));
        moves.pop_back();
    }
}

void ChessWidget::play_random()
{
    std::unordered_set<Ply> possible = state.get_possible_plies();
    if (possible.size() != 0)
    {
        int randidx = rand() % possible.size();
        int counter = 0;
        for (auto &&ply : possible)
        {
            if (counter == randidx)
            {
                moves.push_back(state.get_algebraic_notation(ply));

                sf::Sprite sprite(textures[2 * ply.get_piece() + state.get_turn()]);
                sprite.scale(tile_size / sprite.getTexture()->getSize().x * 0.7f, tile_size / sprite.getTexture()->getSize().y * 0.7f);
                sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);

                state.execute_ply(ply);
                animated.insert(ply.get_destination().get_square());
                history.push(ply);

                animations.push_back(MoveAnimation(0.5f, ply, tile_size, sprite));
                reload();
            }
            counter++;
        }
    }
}

void ChessWidget::reload()
{
    sprites.clear();
    for (uint8_t rank = 0; rank < 8; rank++)
    {
        for (uint8_t file = 0; file < 8; file++)
        {
            Location location(file, rank);
            Color color = state.get_color(location);
            if (color != NoColor && location != selected_location && animated.count(location.get_square()) == 0)
            {
                Piece piece = state.get_piece(location, color);
                sf::Sprite sprite(textures[2 * piece + color]);
                sprite.scale(tile_size / sprite.getTexture()->getSize().x * 0.7f, tile_size / sprite.getTexture()->getSize().y * 0.7f);
                sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
                sprite.setPosition(tile_size * file + tile_size / 2, tile_size * rank + tile_size / 2);
                sprites.push_back(sprite);
            }
        }
    }
}

#include <iostream>

void ChessWidget::mouse_moved(sf::Vector2i position)
{
    int x = (position.x / tile_size);
    int y = (position.y / tile_size);

    if (selected_location.is_valid())
    {
        selected_sprite.setPosition(position.x, position.y);
    }

    //std::cout << x << " " << y << std::endl;
}
void ChessWidget::mouse_pressed(sf::Vector2i position, sf::Mouse::Button button)
{
    int x = (position.x / tile_size);
    int y = (position.y / tile_size);

    if (x >= 0 && x <= 7 && y >= 0 && y <= 7)
    {
        Location location(x, y);
        Color color = state.get_color(location);
        if (color == state.get_turn())
        {
            selected_piece = state.get_piece(location, color);
            std::cout << to_char(color) << " " << piece_names[selected_piece] << std::endl;
            selected_location = Location(x, y);
            selected_sprite = sf::Sprite(textures[2 * selected_piece + color]);
            selected_sprite.scale(tile_size / selected_sprite.getTexture()->getSize().x * 0.7f, tile_size / selected_sprite.getTexture()->getSize().y * 0.7f);
            selected_sprite.setOrigin(selected_sprite.getTextureRect().width / 2.f, selected_sprite.getTextureRect().height / 2.f);
            selected_sprite.setPosition(tile_size * x + tile_size / 2, tile_size * y + tile_size / 2);
            reload();
        }
    }
}
void ChessWidget::mouse_released(sf::Vector2i position, sf::Mouse::Button button)
{
    if (selected_location.is_valid())
    {
        int x = (position.x / tile_size);
        int y = (position.y / tile_size);

        if (x >= 0 && x <= 7 && y >= 0 && y <= 7)
        {
            Location location(x, y);
            Ply ply(selected_piece, selected_location, location);
            if (state.is_possible(ply))
            {
                moves.push_back(state.get_algebraic_notation(ply));
                state.execute_ply(ply);
                animated.insert(ply.get_destination().get_square());
                history.push(ply);
                animations.push_back(MoveAnimation(0.5f, ply, tile_size, selected_sprite));
            }
            selected_location = Location();
            selected_piece = Empty;
            reload();
        }
    }
}