#include <iostream>

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Globals.h"
#include "Managers/AnimationManager.h"
#include "Managers/EditorManager.h"
#include "Managers/GameManager.h"


using json = nlohmann::json;

int main(){
    auto screenDimensions = Globals::getScreenDimensions();
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {static_cast<float>(screenDimensions.x/2),
        static_cast<float>(screenDimensions.y/2)},
        sf::Vector2f(screenDimensions));

    GameManager::initialise(&window, &view);

    while (window.isOpen()) {
        GameManager::update();

        window.clear();
        GameManager::draw();
        window.display();
    }
}