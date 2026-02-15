//
// Created by jay on 26/01/2026.
//

#include "LevelManager.h"

#include "TileManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "GameManager.h"
#include "ManagerUtilities.h"

std::vector<std::string> LevelManager::levels = {};
std::string LevelManager::fullPath;

void LevelManager::initialiseLevels(){
    fullPath = GameManager::getAssetPath() + "levels/";

    levels = ManagerUtilities::findFiles(fullPath, {".json"});
    for (const std::string& level : levels){
        std::cout << "Initialised level: \"" << level << "\"" << std::endl;
    }
}

Level LevelManager::loadLevel(const std::string& name){
    if (!isLevel(name)){
        throw std::runtime_error("Could not find level: \"" + name + "\"");
    }
    Level level;
    LevelJson json = parseLevel(fullPath + name);

    for (const auto& tileData : json.levelData.tiles) {
        level.levelCollisions.emplace_back(getTileCollision(tileData));
        for (const auto& t : getTiles(tileData)) {
            level.tiles.emplace_back(t);
        }
    }

    level.backgroundColour = sf::Color{97,133,248};
    level.playerStartPosition = json.playerData.startPos;

    std::cout << "Loaded level: \"" << name << "\"" << std::endl;
    return level;
}

std::vector<Tile> LevelManager::getTiles(const TileData& data) {
    std::vector<Tile> tiles;
    for (int i = 0; i < data.size.x; i++) {
        for (int j = 0; j < data.size.y; j++) {
            Tile tile(*TileManager::loadTile(data.name));
            const sf::Vector2i gridPosition(data.pos.x + i,data.pos.y + j);
            const sf::Vector2i pixelPosition(gridPosition.x * 16,gridPosition.y * 16);

            tile.setPosition(pixelPosition);
            tile.setTextureIndex(0);
            tiles.emplace_back(tile);
        }
    }
    return tiles;
}

sf::IntRect LevelManager::getTileCollision(const TileData& data) {
    const sf::Vector2i pos({data.pos.x * 16, data.pos.y * 16});
    const sf::Vector2i size({data.size.x * 16, data.size.y * 16});
    const sf::IntRect rect(pos, size);
    return rect;
}

LevelJson LevelManager::parseLevel(const std::string& filePath) {
    std::ifstream levelFileData(filePath);
    nlohmann::json levelFile(nlohmann::json::parse(levelFileData));
    LevelJson data;
    data.playerData = parsePlayerData(levelFile["playerData"]);
    data.levelData = parseLevelData(levelFile["levelData"]);
    return data;
}

PlayerData LevelManager::parsePlayerData(const nlohmann::json& playerData) {
    if (playerData.empty()) {
        throw std::runtime_error("No player data found!");
    }
    PlayerData data;
    data.startPos = ManagerUtilities::getVector2fFromJson(playerData["startPos"]);
    return data;
}

LevelData LevelManager::parseLevelData(const nlohmann::basic_json<>& tilesData) {
    LevelData data;
    for (const auto& tile : tilesData) {
        data.tiles.emplace_back(parseTileData(tile));
    }
    return data;
}

TileData LevelManager::parseTileData(const nlohmann::basic_json<>& tileData) {
    if (tileData.empty()) {
        throw std::runtime_error("No tile data found!");
    }
    TileData data;
    data.name = ManagerUtilities::getStringFromJson(tileData["name"]);
    data.pos = ManagerUtilities::getVector2iFromJson(tileData["pos"]);
    data.size = ManagerUtilities::getVector2iFromJson(tileData["size"]);
    data.skinOverride = tileData["skinOverride"];

    return data;
}

bool LevelManager::isInitialised(){
    return ManagerUtilities::isInitialised(levels);
}

bool LevelManager::isLevel(const std::string& name){
    if (!isInitialised()){
        throw std::runtime_error("No levels initialised!");
    }
    return std::ranges::find(levels, name) != levels.end();
}

