//
// Created by jay on 26/01/2026.
//

#ifndef SFML_MARIO_TEST_LEVELMANAGER_H
#define SFML_MARIO_TEST_LEVELMANAGER_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "../Types/Tile.h"

struct Level{
    std::vector<Tile> tiles;
    std::vector<sf::FloatRect> levelCollisions;
    sf::Vector2f playerStartPosition;
    sf::Color backgroundColour;
};

class LevelManager{
public:
    LevelManager() = default;
    ~LevelManager() = default;

    static void initialiseLevels();
    static bool isLevel(const std::string& name);
    static bool isInitialised();
    static Level loadLevel(const std::string& name);


private:
    static std::vector<std::string> levels;
    static std::string fullPath;
};


#endif //SFML_MARIO_TEST_LEVELMANAGER_H