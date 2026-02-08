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
sf::RenderWindow* GameManager::window;
std::string GameManager::assetPath;

void GameManager::setWindow(sf::RenderWindow& targetWindow){
    window = &targetWindow;
}

void GameManager::initialiseGame(){
    if (assetPath.empty()){
        throw std::runtime_error("Tried to initialise game without present asset path!");
    }
    initialiseGame(assetPath);
}

void GameManager::initialiseGame(const std::string& path){
    assetPath = path;
    std::cout << "Initialising game..." << std::endl;
    initialiseManagers();
    player.initialisePlayer();
    setupLevel();
}

void GameManager::setupLevel(){
    level = LevelManager::loadLevel("1-1.json");
    player.setPosition({level.playerStartPosition});
}

void GameManager::initialiseManagers(){
    AnimationManager::initialiseAnimations();
    TextureManager::initialiseTextures();
    TileManager::initialiseTiles();
    LevelManager::initialiseLevels();
}

void GameManager::checkForEvents(){
    while (const std::optional event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()){
            window->close();
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
}

void GameManager::updateGame(){
    checkForEvents();
    handleInput();

    float deltaTime = clock.restart().asSeconds();
    player.update(deltaTime);

    for (Tile& tile : level.tiles){
        if (tile.isAnimated()){
            tile.update(deltaTime);
        }
    }

    checkForCollisions();
}

void GameManager::handleInput(){
    if (InputManager::isLastKeyPressed(sf::Keyboard::Key::Escape)){
        window->close();
    }
    else if (InputManager::isKeyPressed(sf::Keyboard::Key::F3) && InputManager::isLastKeyPressed(sf::Keyboard::Key::A)){
        std::cout << "Reloading assets..." << std::endl;
        initialiseGame();
    }

    player.handleInput();
}

void GameManager::checkForCollisions(){
    for (auto& collisionBox : level.levelCollisions){
        auto intersection = collisionBox.findIntersection(player.getBoundingBox());
        if (intersection.has_value()){
            sf::FloatRect overlap = intersection.value();
            CollisionSide side = getCollisionSide(player.getBoundingBox(), collisionBox);
            // @TODO USE SWEEPING AABB TO REMOVE TUNNELLING
            player.collide(side, overlap);
        }
    }
}

void GameManager::drawGame(sf::RenderTarget& target){
    target.clear(level.backgroundColour);
    player.draw(target);

    for (const auto& tile : level.tiles){
        tile.draw(target);
    }
}

CollisionSide GameManager::getCollisionSide(const sf::FloatRect& a, const sf::FloatRect& b){
    auto intersection = a.findIntersection(b);
    if (!intersection.has_value()){
        return None;
    }
    sf::FloatRect overlap = intersection.value();

    if (overlap.size.y > overlap.size.x){
        if (overlap.position.x + overlap.size.x < b.getCenter().x){
            return Left;
        }
        if (overlap.position.x + overlap.size.x > b.getCenter().x){
            return Right;
        }
    }
    else if (overlap.size.x > overlap.size.y){
        if (overlap.position.y + overlap.size.y < b.getCenter().y){
            return Top;
        }
        if (overlap.position.y + overlap.size.y > b.getCenter().y){
            return Bottom;
        }
    }
    return None;
}
