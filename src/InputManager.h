//
// Created by jay on 30/01/2026.
//

#ifndef SFML_MARIO_TEST_INPUTMANAGER_H
#define SFML_MARIO_TEST_INPUTMANAGER_H
#include <SFML/Window.hpp>


class InputManager{
public:
    InputManager() = default;
    ~InputManager() = default;

    static bool isLastKeyPressed(sf::Keyboard::Key key);
    static bool isKeyPressed(sf::Keyboard::Key key);

    static sf::Keyboard::Key getLastKeyPressed();
    static void setLastKeyPressed(sf::Keyboard::Key key);
private:
    static sf::Keyboard::Key lastKeyPressed;
};


#endif //SFML_MARIO_TEST_INPUTMANAGER_H