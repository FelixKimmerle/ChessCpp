#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ChessWidget.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 500), "ImGui + SFML = <3");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = nullptr;

    ChessWidget chess_widget(500);
    chess_widget.scale(1.0, -1.0);
    chess_widget.setPosition(0, 500);

    sf::RenderTexture render_texutre;
    render_texutre.create(500, 500);

    const static float ratio = 0.6;

    // sprite.scale(-1.0f,1.0f);
    float menu_bar_size = 25;
    int xoffset = 0;
    int yoffset = 0;

    sf::Clock deltaClock;
    sf::Vector2i mouse_pos;
    char input_buffer[256] = "";
    std::string fen_string;
    bool input_fen = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Z && event.key.control)
                {
                    chess_widget.undo();
                }
                else if (event.key.code == sf::Keyboard::Space)
                {
                    chess_widget.play_random();
                }
            }
            else if (event.type == sf::Event::Resized)
            {
                float size = std::min(event.size.width * ratio, (float)event.size.height - menu_bar_size * 1.50f);
                render_texutre.create(size, size);
                chess_widget.resize(size);
                chess_widget.setPosition(0, size);
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                mouse_pos = sf::Mouse::getPosition(window);
                mouse_pos -= sf::Vector2i(xoffset / 2, menu_bar_size + yoffset);
                chess_widget.mouse_moved(mouse_pos);
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                chess_widget.mouse_pressed(mouse_pos, event.mouseButton.button);
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                chess_widget.mouse_released(mouse_pos, event.mouseButton.button);
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        chess_widget.update();

        render_texutre.clear(sf::Color::Green);
        render_texutre.draw(chess_widget);
        render_texutre.display();

        ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowSize(window.getSize());
        ImGui::SetWindowPos(ImVec2(0, 0));

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Load"))
            {
                memset(input_buffer, 0, sizeof(input_buffer));
                input_fen = true;

                // chess_widget.load_fen();
            }

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
            if (input_fen)
            {
                ImGui::Begin("FEN Input", &input_fen, window_flags);
                if (ImGui::InputText("Enter FEN", input_buffer, 256))
                {
                    fen_string = input_buffer;
                }
                // ImGui::Text("Current FEN: %s", fen_string.c_str());
                if(ImGui::Button("Cancel"))
                {
                    input_fen = false;
                }
                ImGui::SameLine();
                if(ImGui::Button("Ok"))
                {
                    chess_widget.load_fen(fen_string);
                    input_fen = false;
                    fen_string = "";
                }
                ImGui::End();
            }
            ImGui::EndMenuBar();
        }

        menu_bar_size = ImGui::GetFrameHeightWithSpacing();
        xoffset = ImGui::GetWindowWidth() - ImGui::GetWindowContentRegionWidth();
        yoffset = ImGui::GetFrameHeightWithSpacing() - ImGui::GetFrameHeight();

        ImGui::Image(render_texutre.getTexture());
        ImGui::SameLine();
        ImGui::BeginChildFrame(1, ImVec2(ImGui::GetWindowContentRegionWidth() - render_texutre.getSize().x - xoffset / 2.0f, window.getSize().y - menu_bar_size * 2.0f + yoffset / 2.0f), ImGuiWindowFlags_NoBackground);
        chess_widget.draw_gui();
        ImGui::EndChildFrame();
        ImGui::End();

        // ImGui::ShowDemoWindow();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
