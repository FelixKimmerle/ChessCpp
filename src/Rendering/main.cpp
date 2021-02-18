#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ChessWidget.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    ChessWidget chess_widget(400);
    chess_widget.scale(1.0, -1.0);
    chess_widget.setPosition(0, 400);

    sf::RenderTexture render_texutre;
    render_texutre.create(500, 500);
    //sprite.scale(-1.0f,1.0f);
    const static float ratio = 0.6;
    float menu_bar_size = 0;
    int xoffset = 0;
    int yoffset = 0;

    sf::Clock deltaClock;
    sf::Vector2i mouse_pos;

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
                else if(event.key.code == sf::Keyboard::Z && event.key.control)
                {
                    chess_widget.undo();
                }
                else if(event.key.code == sf::Keyboard::Space)
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

        //ImGui::ShowDemoWindow();

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
            }
            ImGui::EndMenuBar();
        }

        menu_bar_size = ImGui::GetFrameHeightWithSpacing();
        xoffset = ImGui::GetWindowWidth() - ImGui::GetWindowContentRegionWidth();
        yoffset = ImGui::GetFrameHeightWithSpacing() - ImGui::GetFrameHeight();

        ImGui::Image(render_texutre.getTexture());
        ImGui::SameLine();
        ImGui::BeginChildFrame(1, ImVec2(ImGui::GetWindowContentRegionWidth() * (1.0f - ratio), window.getSize().y - menu_bar_size * 1.50f), ImGuiWindowFlags_NoBackground);
        chess_widget.draw_gui();
        ImGui::EndChildFrame();
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
