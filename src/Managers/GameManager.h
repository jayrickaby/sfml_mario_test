//
// Created by jay on 26/02/2026.
//

#ifndef SFML_MARIO_TEST_GAMEMANAGER_H
#define SFML_MARIO_TEST_GAMEMANAGER_H
#include "SFML/Graphics.hpp"
#include "../Types/Level.h"


class GameManager {
public:
    static void initialise(sf::RenderWindow* targetWindow, sf::View* targetView);
    static void update();
    static void draw();

    // Validators
    static bool isInitialised();

    // Getters
    static Level* getLevel();

    // Setters
private:
    static bool initialised;
    static Level level;

    static sf::RenderWindow* window;
    static sf::View* view;
};


#endif //SFML_MARIO_TEST_GAMEMANAGER_H