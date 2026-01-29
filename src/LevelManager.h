//
// Created by jay on 26/01/2026.
//

#ifndef SFML_MARIO_TEST_LEVELMANAGER_H
#define SFML_MARIO_TEST_LEVELMANAGER_H
#include <string>
#include <vector>

#include "Tile.h"

struct Level{
    std::vector<Tile> tiles;
    sf::Vector2f playerStartPosition;
    sf::Color backgroundColour;
};

class LevelManager{
public:
    LevelManager() = default;
    ~LevelManager() = default;

    static void initialiseLevels();
    static void isLevel(const std::string& name);
    static bool isInitialised();
    static Level loadLevel(const std::string& name);

private:
    static std::vector<std::string> levels;
    static std::string levelDirectory;
};


#endif //SFML_MARIO_TEST_LEVELMANAGER_H