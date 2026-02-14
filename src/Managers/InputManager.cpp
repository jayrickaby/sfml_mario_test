//
// Created by jay on 30/01/2026.
//

#include "InputManager.h"

InputButton InputManager::lastButtonPressed = InputButton::Unknown;
InputMethod InputManager::inputMethod = InputMethod::Unknown;
std::vector<InputButton> InputManager::buttonsPressed;
sf::Vector2f InputManager::axesPosition = {0,0};
float InputManager::axisDeadzone = 20.f;

//@TODO Load from config file
std::map<sf::Keyboard::Key, InputButton> InputManager::keysToButtons = {
    {sf::Keyboard::Key::A, InputButton::Left},
    {sf::Keyboard::Key::D, InputButton::Right},
    {sf::Keyboard::Key::W, InputButton::Up},
    {sf::Keyboard::Key::S, InputButton::Down},
    {sf::Keyboard::Key::Space, InputButton::A}
};

std::map<int, InputButton> InputManager::joystickButtonToButtons = {
    {0, InputButton::A}
};

void InputManager::initialiseInput() {
    if (sf::Joystick::isConnected(0)) {
        setInputMethod(InputMethod::Controller);
    }
    else {
        setInputMethod(InputMethod::Keyboard);
    }
}

InputButton InputManager::getLastButtonPressed(){
    return lastButtonPressed;
}

bool InputManager::isButtonPressed(const InputButton& button) {
    for (const auto& b : buttonsPressed) {
        if (b == button) {
            return true;
        }
    }
    return false;
}

void InputManager::setInputMethod(const InputMethod& input) {
    inputMethod = input;
}

void InputManager::pressButton(const InputButton& button) {
    lastButtonPressed = button;
    buttonsPressed.push_back(button);
}

void InputManager::releaseButton(const InputButton& button) {
    if (lastButtonPressed == button) {
        lastButtonPressed = InputButton::Unknown;
    }
    std::erase(buttonsPressed, button);
}

void InputManager::moveAxes(const float& x, const float& y) {
    axesPosition = {x,y};
    if (x > axisDeadzone) {
        lastButtonPressed = InputButton::Right;
        if (!isButtonPressed(InputButton::Right)) {
            buttonsPressed.emplace_back(InputButton::Right);
        }
        std::erase(buttonsPressed, InputButton::Left);
    }
    else if (x < -axisDeadzone) {
        lastButtonPressed = InputButton::Left;
        if (!isButtonPressed(InputButton::Left)) {
            buttonsPressed.emplace_back(InputButton::Left);
        }
        buttonsPressed.emplace_back(InputButton::Right);
        std::erase(buttonsPressed, InputButton::Right);
    }
    else {
        std::erase(buttonsPressed, InputButton::Left);
        std::erase(buttonsPressed, InputButton::Right);
        if (getLastButtonPressed() == InputButton::Left || getLastButtonPressed() == InputButton::Right) {
            lastButtonPressed = InputButton::Unknown;
        }
    }
}

bool InputManager::isLastButtonPressed(const InputButton& button) {
    return lastButtonPressed == button;
}

InputMethod InputManager::getInputMethod() {
    return inputMethod;
}