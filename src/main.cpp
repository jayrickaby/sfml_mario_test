#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Globals.h"
#include "Managers/AnimationManager.h"
#include "Managers/ModelManager.h"
#include "Managers/TextureManager.h"
#include "Managers/TileManager.h"
#include "Types/Tile.h"

using json = nlohmann::json;

int main(){
    auto screenDimensions = Globals::getScreenDimensions();
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {screenDimensions.x/2,screenDimensions.y/2}, screenDimensions);

    AnimationManager::initialise();
    TextureManager::initialise();
    ModelManager::initialise();
    TileManager::initialise();

    Tile tile = TileManager::getTile("questionblock");

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        Globals::updateDeltaTime();
        tile.update();

        window.clear();
        window.setView(view);
        tile.draw(window);
        window.display();
    }
}