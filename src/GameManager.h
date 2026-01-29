//
// Created by jay on 29/01/2026.
//

#ifndef SFML_MARIO_TEST_GAMEMANAGER_H
#define SFML_MARIO_TEST_GAMEMANAGER_H
#include <string>
#include <SFML/Graphics.hpp>

#include "LevelManager.h"
#include "Player.h"


class GameManager{
public:
    GameManager() = default;
    ~GameManager() = default;

    static void initialiseGame(const std::string& path);
    static void initialiseGame();
    static void updateGame(sf::RenderWindow& window);
    static void drawGame(sf::RenderTarget& target);

    // Getter
    static std::string getAssetPath() { return assetPath; }

private:
    static sf::Clock clock;
    static Player player;
    static Level level;

    static std::string assetPath;
};


#endif //SFML_MARIO_TEST_GAMEMANAGER_H