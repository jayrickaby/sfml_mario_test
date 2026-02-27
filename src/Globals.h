//
// Created by jay on 09/02/2026.
//

#ifndef SFML_MARIO_TEST_GLOBALS_H
#define SFML_MARIO_TEST_GLOBALS_H
#include <SFML/Graphics.hpp>


class Globals{
public:
    Globals() = default;
    ~Globals() = default;

    // Getters
    static float getDeltaTime();
    static sf::Vector2i getMousePosition() {return mousePosition;};
    static sf::Vector2i getScreenDimensions() {return {SCREEN_WIDTH, SCREEN_HEIGHT};}

    // Updater
    static void update(const sf::RenderWindow* window);

private:
    // Globals
    static float deltaTime;
    static sf::Vector2i mousePosition;
    static sf::Clock clock;

    // Consts
    static constexpr int SCREEN_WIDTH = 256;
    static constexpr int SCREEN_HEIGHT = 256;
    static constexpr float EPSILON = 0.0001;
};


#endif //SFML_MARIO_TEST_GLOBALS_H