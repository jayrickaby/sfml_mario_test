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
    static sf::Vector2f getScreenDimensions() {return sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT);}
    static float getDeltaTime();

    // Updater
    static void updateDeltaTime();

private:
    // Globals
    static float deltaTime;
    static sf::Clock clock;

    // Consts
    static constexpr int SCREEN_WIDTH = 256;
    static constexpr int SCREEN_HEIGHT = 256;
    static constexpr float EPSILON = 0.0001;
};


#endif //SFML_MARIO_TEST_GLOBALS_H