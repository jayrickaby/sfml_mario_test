//
// Created by jay on 09/01/2026.
//

#ifndef SFML_MARIO_TEST_PLAYER_H
#define SFML_MARIO_TEST_PLAYER_H
#include <SFML/Graphics.hpp>

class Player{
public:
    Player();
    void render();
    void update();
    void handleInput();

private:
    // Appearance
    sf::Texture* texture;
    sf::Sprite sprite;

    // Physics
    float speed;
    float gravity;
    float jumpStrength;
    sf::Vector2f velocity;
    sf::FloatRect physicsBox;
};

#endif //SFML_MARIO_TEST_PLAYER_H