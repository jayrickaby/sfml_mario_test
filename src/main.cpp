#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>


#include "AnimationManager.h"
#include "constants.h"
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

    Tile groundTile = *TileManager::loadTile("ground");

    std::vector<Tile> tiles;
    for (int i = 0; i < 16; i++){
        Tile groundBlock = *TileManager::loadTile("ground");
        groundBlock.setTextureIndex(1);
        groundBlock.setPosition({i * 16.f, SCREEN_HEIGHT - 16.f});
        tiles.push_back(groundBlock);
    }

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

        for (const auto& tile : tiles){
            tile.draw(window);
        }

        window.display();
    }
    return 0;
}
