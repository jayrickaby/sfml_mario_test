#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "constants.h"
#include "GameManager.h"
#include "InputManager.h"

using json = nlohmann::json;

int main(){
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {SCREEN_WIDTH/2,SCREEN_HEIGHT/2}, sf::Vector2f{SCREEN_WIDTH,SCREEN_HEIGHT});

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
