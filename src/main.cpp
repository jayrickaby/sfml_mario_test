#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Managers/GameManager.h"
#include "Globals.h"

using json = nlohmann::json;

int main(){
    auto screenDimensions = Globals::getScreenDimensions();
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {screenDimensions.x/2,screenDimensions.y/2}, screenDimensions);

    GameManager::setWindow(window);
    GameManager::initialiseGame("assets/");

    while (window.isOpen()){
        GameManager::updateGame();
        window.setView(view);
        GameManager::drawGame(window);
        window.display();
    }

    return 0;
}
