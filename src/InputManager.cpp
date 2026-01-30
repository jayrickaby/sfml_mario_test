//
// Created by jay on 30/01/2026.
//

#include "InputManager.h"

sf::Keyboard::Key InputManager::lastKeyPressed = sf::Keyboard::Key::Unknown;

bool InputManager::isLastKeyPressed(sf::Keyboard::Key key){
    return lastKeyPressed == key;
}

bool InputManager::isKeyPressed(const sf::Keyboard::Key key){
    return sf::Keyboard::isKeyPressed(key);
}

void InputManager::setLastKeyPressed(const sf::Keyboard::Key key){
    lastKeyPressed = key;
}

sf::Keyboard::Key InputManager::getLastKeyPressed(){
    return lastKeyPressed;
}