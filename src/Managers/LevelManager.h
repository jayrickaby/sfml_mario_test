//
// Created by jay on 26/01/2026.
//

#ifndef SFML_MARIO_TEST_LEVELMANAGER_H
#define SFML_MARIO_TEST_LEVELMANAGER_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "../Types/Tile.h"

struct PlayerData {
    sf::Vector2f startPos;
};

struct TileData {
    std::string name;
    sf::Vector2i pos;
    sf::Vector2i size;
    int skinOverride;
};

struct LevelData {
    std::vector<TileData> tiles;
};

struct LevelJson {
    PlayerData playerData;
    LevelData levelData;
};

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

    static LevelJson parseLevel(const std::string& name);
    static PlayerData parsePlayerData(const nlohmann::basic_json<>& playerData);
    static LevelData parseLevelData(const nlohmann::basic_json<>& tilesData);
    static TileData parseTileData(const nlohmann::basic_json<>& tileData);

    static Level loadLevel(const std::string& name);
    static std::vector<Tile> getTiles(const TileData& data);
    static sf::IntRect getTileCollision(const TileData& data);


private:
    static std::vector<std::string> levels;
    static std::string fullPath;
};


#endif //SFML_MARIO_TEST_LEVELMANAGER_H