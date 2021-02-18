#pragma once
#include <stack>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "Chess/State.hpp"
#include "MoveAnimation.hpp"

class ChessWidget : public sf::Drawable, public sf::Transformable
{
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray light_square;
    sf::VertexArray dark_square;

    sf::Texture textures[12];
    sf::Font font;
    sf::Text texts[64];

    std::vector<sf::Sprite> sprites;

    float tile_size;
    float light_square_color[3];
    float dark_square_color[3];
    float text_color[3];

    Location selected_location;
    Piece selected_piece;
    sf::Sprite selected_sprite;

    std::vector<MoveAnimation> animations;
    std::unordered_set<uint8_t> animated;
    std::vector<std::string> moves;

    bool new_move;

    State state;
    void reload();

    bool draw_text;
    bool draw_animations;
    float animation_speed;

    std::vector<sf::Texture> texure_history;

    std::stack<Ply> history;
public:
    ChessWidget(float size);
    ~ChessWidget();

    void resize(float size);
    void draw_gui();
    void update();
    void undo();
    void play_random();

    void mouse_moved(sf::Vector2i position);
    void mouse_pressed(sf::Vector2i position, sf::Mouse::Button button);
    void mouse_released(sf::Vector2i position, sf::Mouse::Button button);

};
