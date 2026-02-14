//
// Created by jay on 30/01/2026.
//

#ifndef SFML_MARIO_TEST_INPUTMANAGER_H
#define SFML_MARIO_TEST_INPUTMANAGER_H
#include <map>
#include <SFML/Window.hpp>

enum class InputMethod {
    Keyboard,
    Controller,
    Unknown
};

enum class InputButton {
    Left,
    Right,
    Up,
    Down,
    A,
    B,
    X,
    Y,
    Unknown
};

class InputManager{
public:
    InputManager() = default;
    ~InputManager() = default;
    static void initialiseInput();

    static std::map<sf::Keyboard::Key, InputButton> keysToButtons;
    static std::map<int, InputButton> joystickButtonToButtons;

    // Validators
    static bool isLastButtonPressed(const InputButton& button);
    static bool isButtonPressed(const InputButton& button);

    // Getters
    static InputButton getLastButtonPressed();
    static InputMethod getInputMethod();

    // Setters
    static void pressButton(const InputButton& button);
    static void releaseButton(const InputButton& button);
    static void moveAxes(const float& x, const float& y);
    static void setInputMethod(const InputMethod& input);
private:
    static InputButton lastButtonPressed;
    static InputMethod inputMethod;
    static std::vector<InputButton> buttonsPressed;
    static sf::Vector2f axesPosition;
    static float axisDeadzone;
};


#endif //SFML_MARIO_TEST_INPUTMANAGER_H