#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>


#include "AnimationManager.h"
#include "constants.h"
#include "LevelManager.h"
#include "Player.h"
#include "TextureManager.h"
#include "TileManager.h"

using json = nlohmann::json;

int main(){
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {SCREEN_WIDTH/2,SCREEN_HEIGHT/2}, sf::Vector2f{SCREEN_WIDTH,SCREEN_HEIGHT});

    AnimationManager::initialiseAnimations("assets/animations/");
    TextureManager::initialiseTextures("assets/textures/");
    TileManager::initialiseTiles("assets/models/");
    LevelManager::initialiseLevels("assets/levels/");

    Level level = LevelManager::loadLevel("1-1");

    Player player;
    player.setPosition({0,0});
    sf::Clock clock;

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear();

        window.setView(view);
        player.update(deltaTime);

        player.draw(window);

        for (const auto& tile : level.tiles){
            tile.draw(window);
        }

        window.display();
    }
    return 0;
}
