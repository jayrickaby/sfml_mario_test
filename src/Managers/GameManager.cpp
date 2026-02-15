//
// Created by jay on 29/01/2026.
//

#include "GameManager.h"

#include <iostream>

#include "AnimationManager.h"
#include "../Globals.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "TileManager.h"

Player GameManager::player;
Level GameManager::level;
sf::RenderWindow* GameManager::window;
std::string GameManager::assetPath;
std::vector<sf::RectangleShape> GameManager::debugHitboxes;

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
    SoundManager::initialiseSoundFiles();
    InputManager::initialiseInput();
}

void GameManager::checkForEvents(){
    while (const std::optional event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()){
            window->close();
        }
        //@TODO branch off into subfunctions? maybe InputManager::checkForKeyboardEvents(); etc
        auto inputMethod = InputManager::getInputMethod();
        if (inputMethod == InputMethod::Keyboard) {
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (InputManager::keysToButtons.contains(keyPressed->code)) {
                    InputManager::pressButton(InputManager::keysToButtons.at(keyPressed->code));
                }
            }
            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                if (InputManager::keysToButtons.contains(keyReleased->code)) {
                    InputManager::releaseButton(InputManager::keysToButtons.at(keyReleased->code));
                }
            }
        }
        if (inputMethod == InputMethod::Controller) {
            if (const auto* buttonPressed = event->getIf<sf::Event::JoystickButtonPressed>()){
                if (InputManager::joystickButtonToButtons.contains(buttonPressed->button)) {
                    InputManager::pressButton(InputManager::joystickButtonToButtons.at(buttonPressed->button));
                }
            }
            else if (const auto* buttonReleased = event->getIf<sf::Event::JoystickButtonReleased>()) {
                if (InputManager::joystickButtonToButtons.contains(buttonReleased->button)) {
                    InputManager::releaseButton(InputManager::joystickButtonToButtons.at(buttonReleased->button));
                }
            }
            if (const auto* axisPressed = event->getIf<sf::Event::JoystickMoved>()) {
                if (axisPressed->axis == sf::Joystick::Axis::X) {
                    InputManager::moveAxes(axisPressed->position, 0.f);
                }
                if (axisPressed->axis == sf::Joystick::Axis::Y) {
                    std::cout << "Y: " << axisPressed->position << std::endl;
                }
            }
        }
    }
}

void GameManager::updateGame(){
    checkForEvents();
    handleInput();

    SoundManager::clearStoppedSounds();
    Globals::updateDeltaTime();
    player.update();

    for (auto& tile : level.tiles){
        if (tile.isAnimated()){
            tile.update();
        }
    }

    checkForCollisions();

    std::vector<sf::RectangleShape> vector;
    sf::RectangleShape rect(player.getBoundingBox().size);
    rect.setPosition(player.getPosition());
    rect.setFillColor(sf::Color::White);
    vector.emplace_back(rect);

    for (const auto& box : level.levelCollisions) {
        sf::RectangleShape rectangle_shape(sf::Vector2f(box.size));
        rectangle_shape.setPosition(sf::Vector2f(box.position));
        rectangle_shape.setFillColor(sf::Color::Red);
        rectangle_shape.setOutlineColor(sf::Color::Black);
        rectangle_shape.setOutlineThickness(1);
        vector.emplace_back(rectangle_shape);
    }

    debugHitboxes = vector;
}

void GameManager::handleInput(){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
        window->close();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        std::cout << "Reloading assets..." << std::endl;
        initialiseGame();
    }

    player.handleInput();
}

void GameManager::checkForCollisions(){
    for (auto& collisionBox : level.levelCollisions){
        auto intersection = collisionBox.findIntersection(player.getBoundingBox());
        if (intersection.has_value()){
            const sf::FloatRect overlap = intersection.value();
            const CollisionSide side = getCollisionSide(player.getBoundingBox(), collisionBox);
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

    for (const auto& hitbox : debugHitboxes) {
        //window->draw(hitbox);
    }
}

CollisionSide GameManager::getCollisionSide(const sf::FloatRect& a, const sf::FloatRect& b){
    const auto intersection = a.findIntersection(b);
    if (!intersection.has_value()){
        return None;
    }
    const auto overlap = intersection.value();

    if (overlap.size.y > overlap.size.x){
        if (overlap.position.x + overlap.size.x < b.getCenter().x){
            return Right;
        }
        if (overlap.position.x + overlap.size.x > b.getCenter().x){
            return Left;
        }
    }
    else if (overlap.size.x > overlap.size.y){
        if (overlap.position.y + overlap.size.y < b.getCenter().y){
            return Bottom;
        }
        if (overlap.position.y + overlap.size.y > b.getCenter().y){
            return Top;
        }
    }
    return None;
}
