#include "ChessWidget.hpp"

#include <sstream>
#include <cmath>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui-SFML.h"

static const std::string piece_names[] = {"queen", "knight", "bishop", "rook", "pawn", "king"};

ChessWidget::ChessWidget(float size) : light_square(sf::Quads, 4),
                                       dark_square(sf::Quads, 4),
                                       light_square_color{165.f / 255.f, 106.f / 255.f, 23.f / 255.f},
                                       dark_square_color{86.f / 255.f, 38.f / 255.f, 20.f / 255.f},
                                       text_color{126.f / 255.f, 255.f / 255.f, 251.f / 255.f},
                                    //    state("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"),
                                       state("3q4/4P1P1/8/8/8/8/4p1p1/8 w - - 0 1"),
                                       selected_piece(Empty),
                                       new_move(true),
                                       draw_text(true),
                                       draw_animations(true),
                                       animation_speed(0.5f)
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

    font.loadFromFile("res/arial.ttf");

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
    if (draw_text)
    {
        for (auto &&text : texts)
        {
            target.draw(text, states);
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

    if (draw_animations)
    {
        for (auto &&animation : animations)
        {
            target.draw(animation.second, states);
        }
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

    for (uint8_t rank = 0; rank < 8; rank++)
    {
        for (uint8_t file = 0; file < 8; file++)
        {
            Location location(file, rank);
            texts[location.get_square()].setFont(font);
            std::stringstream ss;
            ss << location;
            texts[location.get_square()].setString(ss.str());
            texts[location.get_square()].setCharacterSize(tile_size / 5 + 1);
            texts[location.get_square()].setFillColor(sf::Color(text_color[0] * 255, text_color[1] * 255, text_color[2] * 255));
            texts[location.get_square()].setPosition(static_cast<int>(file * tile_size + tile_size / 50.0f), rank * tile_size);
        }
    }

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
    if (draw_text)
    {
        if (ImGui::ColorEdit3("Text color", text_color))
        {
            sf::Color color(text_color[0] * 255, text_color[1] * 255, text_color[2] * 255);

            for (uint8_t i = 0; i < 64; i++)
            {
                texts[i].setFillColor(color);
            }
        }
    }
    ImGui::Checkbox("Draw Text", &draw_text);

    if (ImGui::Checkbox("Draw Animations", &draw_animations))
    {
        animations.clear();
        animated.clear();
        reload();
    }
    if (draw_animations)
    {
        ImGui::SliderFloat("Time", &animation_speed, 0.01f, 1.0f);
    }
    static char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", latest_fen.c_str());
    ImGui::InputText("Fen: ", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Moves:");
    if (ImGui::BeginTable("Header", 2, ImGuiTableFlags_Borders))
    {
        ImGui::TableNextColumn();
        ImGui::Text("White");
        ImGui::TableNextColumn();
        ImGui::Text("Black");
        ImGui::EndTable();
    }
    uint32_t counter = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8, 8));
    if (ImGui::BeginTable("Moves", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY))
    {
        for (auto it = moves.begin(); it != moves.end(); it++)
        {
            if (counter % 2 == 0)
            {
                ImGui::TableNextRow();
            }
            ImGui::TableNextColumn();
            ImGui::Text(it->c_str());

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Image(texure_history[counter]);
                ImGui::EndTooltip();
            }
            counter++;
        }
        if (new_move)
        {
            ImGui::SetScrollHereY();
            new_move = false;
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}

void ChessWidget::update()
{
    // Update each animation using iterators
    for (auto it = animations.begin(); it != animations.end(); ++it)
    {
        it->second.update();
    }

    bool removed = false;

    // Iterate with erasure of finished animations
    for (auto it = animations.begin(); it != animations.end();)
    {
        if (it->second.is_finished())
        {
            // Erase from 'animated' set and remove the animation
            animated.erase(it->second.get_destination());
            it = animations.erase(it); // 'erase' returns the next valid iterator
            removed = true;
        }
        else
        {
            ++it; // Only increment if not erased
        }
    }

    // Reload if any animations were removed
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
        texure_history.pop_back();
        state.undo_ply(ply);
        Ply reverse(ply.get_piece(), ply.get_destination(), ply.get_source());

        sf::Sprite sprite(textures[2 * ply.get_piece() + state.get_turn()]);
        sprite.scale(tile_size / sprite.getTexture()->getSize().x * 0.7f, tile_size / sprite.getTexture()->getSize().y * 0.7f);
        sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
        if (draw_animations)
        {
            add_animation(animation_speed, reverse, tile_size, sprite);
        }
        moves.pop_back();
        new_move = true;
        reload();
    }
}

void ChessWidget::play_random()
{
    std::unordered_set<Ply> possible = state.get_possible_plies();
    if (possible.size() != 0)
    {
        // for (const auto &ply : possible)
        // {
        //     std::cout << (ply) << std::endl;
        // }

        int randidx = rand() % possible.size();
        int counter = 0;
        for (auto &&ply : possible)
        {
            if (counter == randidx)
            {
                std::cout << "Played: " << state.get_algebraic_notation(ply) << " (" << ply << ")" << std::endl;
                moves.push_back(state.get_algebraic_notation(ply));
                new_move = true;

                sf::Sprite sprite(textures[2 * ply.get_piece() + state.get_turn()]);
                sprite.scale(tile_size / sprite.getTexture()->getSize().x * 0.7f, tile_size / sprite.getTexture()->getSize().y * 0.7f);
                sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);

                state.execute_ply(ply);
                history.push(ply);

                if (draw_animations)
                {
                    add_animation(animation_speed, ply, tile_size, sprite);
                }

                sf::RenderStates states;
                float size = 350;
                states.transform.scale(size / (tile_size * 8.0f), size / (tile_size * 8.0f));
                states.transform.scale(1.f, -1.f);
                states.transform.translate(0, -(tile_size * 8.0f));
                static sf::RenderTexture tool_tip_texture;
                tool_tip_texture.create(size, size);
                tool_tip_texture.setSmooth(true);
                tool_tip_texture.clear(sf::Color::Green);

                bool old_draw_animations = draw_animations;
                bool old_draw_text = draw_text;
                draw_animations = false;
                draw_text = false;
                reload();
                tool_tip_texture.draw(*this, states);
                draw_animations = old_draw_animations;
                draw_text = old_draw_text;
                tool_tip_texture.display();
                texure_history.push_back(tool_tip_texture.getTexture());
                reload();
                // std::cout << state << std::endl;
                std::cout << animated.size() << std::endl;
            }
            counter++;
        }
    }
}
void ChessWidget::load_fen(const std::string &fen)
{
    state = State(fen);
    reload();
}
void ChessWidget::add_animation(float speed, Ply ply, float tile_size, sf::Sprite &sprite)
{
    auto result = animations.find(ply.get_source().get_square());
    if (result != animations.end())
    {
        // Overwrite the existing value
        animations.erase(result);
        animated.erase(ply.get_source().get_square());
    }
    else
    {
        // Insert a new value if the key does not exist
        animated.insert(ply.get_destination().get_square());
        MoveAnimation animation(speed, ply, tile_size, sprite);
        auto dest_square = ply.get_destination().get_square();
        animations.insert(std::make_pair(dest_square, std::move(animation)));
    }
}

void ChessWidget::reload()
{
    latest_fen = state.to_fen();
    sprites.clear();
    for (uint8_t rank = 0; rank < 8; rank++)
    {
        for (uint8_t file = 0; file < 8; file++)
        {
            Location location(file, rank);
            Color color = state.get_color(location);
            if (color != NoColor && location != selected_location && (!draw_animations || animated.count(location.get_square()) == 0))
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

void ChessWidget::mouse_moved(sf::Vector2i position)
{
    int x = (position.x / tile_size);
    int y = (position.y / tile_size);

    if (selected_location.is_valid())
    {
        selected_sprite.setPosition(position.x, position.y);
    }
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
            uint8_t promotion_rank = state.get_turn() == Color::White ? 7 : 0;
            PlyType plytype = PlyType::Normal;
            if(location.get_rank() == promotion_rank && selected_piece == Piece::Pawn)
            {
                plytype = PlyType::PromoteQueen;
            }
            Ply ply(selected_piece, selected_location, location, plytype);

            if (state.is_possible(ply))
            {
                moves.push_back(state.get_algebraic_notation(ply));
                new_move = true;
                state.execute_ply(ply);
                history.push(ply);
                if (draw_animations)
                {
                    add_animation(animation_speed, ply, tile_size, selected_sprite);
                }
            }
            selected_location = Location();
            selected_piece = Empty;
            reload();
        }
    }
}