//
// Created by jay on 29/01/2026.
//

#ifndef SFML_MARIO_TEST_GAMEMANAGER_H
#define SFML_MARIO_TEST_GAMEMANAGER_H
#include <string>
#include <SFML/Graphics.hpp>

#include "CollisionSide.h"
#include "LevelManager.h"
#include "Player.h"

class GameManager{
public:
    GameManager() = default;
    ~GameManager() = default;

    static void setWindow(sf::RenderWindow& targetWindow);
    static void initialiseGame(const std::string& path);
    static void initialiseGame();
    static void updateGame();
    static void drawGame(sf::RenderTarget& target);

    static CollisionSide getCollisionSide(const sf::FloatRect& a, const sf::FloatRect& b);

    static std::string getAssetPath() { return assetPath; }

private:
    static Player player;
    static Level level;

    static sf::RenderWindow* window;

    static std::string assetPath;

    static void initialiseManagers();
    static void checkForCollisions();
    static void handleInput();
    static void checkForEvents();
    static void setupLevel();
};


#endif //SFML_MARIO_TEST_GAMEMANAGER_H