//
// Created by jay on 29/01/2026.
//

#include "GameManager.h"

#include <iostream>

#include "AnimationManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "TileManager.h"

sf::Clock GameManager::clock;
Player GameManager::player;
Level GameManager::level;
std::string GameManager::assetPath;

void initialiseGame(const std::string& assetPath);
void updateGame(sf::RenderWindow& window);
void drawGame(sf::RenderTarget& target);

void GameManager::initialiseGame(const std::string& path){
    assetPath = path;
    std::cout << "Initialising game..." << std::endl;
    AnimationManager::initialiseAnimations();
    TextureManager::initialiseTextures();
    TileManager::initialiseTiles();
    LevelManager::initialiseLevels();
    player.initialisePlayer();

    level = LevelManager::loadLevel("1-1");

    player.setPosition({level.playerStartPosition});
}

void GameManager::initialiseGame(){
    if (assetPath.empty()){
        throw std::runtime_error("Tried to initialise game without present asset path!");
    }
    initialiseGame(assetPath);
}

void GameManager::updateGame(sf::RenderWindow& window){
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()){
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            InputManager::setLastKeyPressed(keyPressed->code);
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()){
            if (InputManager::isLastKeyPressed(keyReleased->code)){
                InputManager::setLastKeyPressed(sf::Keyboard::Key::Unknown);
            }
        }
    }

    if (InputManager::isLastKeyPressed(sf::Keyboard::Key::Escape)){
        window.close();
    }
    else if (InputManager::isKeyPressed(sf::Keyboard::Key::F3) && InputManager::isLastKeyPressed(sf::Keyboard::Key::A)){
        std::cout << "Reloading assets..." << std::endl;
        initialiseGame();
    }

    float deltaTime = clock.restart().asSeconds();
    player.update(deltaTime);
}

void GameManager::drawGame(sf::RenderTarget& target){
    target.clear(level.backgroundColour);
    player.draw(target);

    for (const auto& tile : level.tiles){
        tile.draw(target);
    }
}
