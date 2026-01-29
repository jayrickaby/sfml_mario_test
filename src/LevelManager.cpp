//
// Created by jay on 26/01/2026.
//

#include "LevelManager.h"

#include "TileManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "GameManager.h"

std::vector<std::string> LevelManager::levels = {};
std::string LevelManager::levelDirectory;

void LevelManager::initialiseLevels(){
    levelDirectory = GameManager::getAssetPath() + "levels/";
    if (!std::filesystem::is_directory(levelDirectory)){
        throw std::runtime_error("Could not find directory: \"" + levelDirectory + "\"");
    }
    if (!TileManager::isInitialised()){
        throw std::runtime_error("Cannot initialise levels before initialising tiles!");
    }
    const std::filesystem::path directory{levelDirectory};

    for (auto const& dirEntry : std::filesystem::directory_iterator{directory}){
        auto levelPath = dirEntry.path().string();

        if (dirEntry.path().extension() == ".json"){
            levels.emplace_back(dirEntry.path().stem().string());
            std::cout << "Initialised level: \"" << dirEntry.path().stem().string() << "\"" << std::endl;
        }
        else{
            std::cout << "Incompatible level file: \"" << dirEntry.path().string() << "\"" << std::endl;
        }
    }
}

Level LevelManager::loadLevel(const std::string& name){
    Level level;
    std::string path = levelDirectory + name + ".json";
    std::ifstream levelFileData(path);
    nlohmann::json file = nlohmann::json::parse(levelFileData);
    if (file["backgroundOverride"] == -1){
        level.backgroundColour = sf::Color{97,133,248};
    }
    const auto& playerData = file["playerData"];
    if (playerData.empty()){
        throw std::runtime_error("Player data in level not defined!");
    }
    std::cout << playerData << std::endl;
    if (playerData["pos"].empty()){
        throw std::runtime_error("Player position in level not defined!");
    }
    sf::Vector2f playerStartPosition = {playerData["pos"][0], playerData["pos"][1]};;
    level.playerStartPosition = {playerStartPosition.x * 16.f, playerStartPosition.y * 16.f};


    for (const auto& tileData : file["tileData"]){
        sf::Vector2f gridPosition({tileData["pos"][0], tileData["pos"][1]});
        sf::Vector2f repeatSize({tileData["size"][0], tileData["size"][1]});

        for (int i = 0; i < repeatSize.x; i++){
            for (int j = 0; j < repeatSize.y; j++){
                sf::Vector2f blockOffset = sf::Vector2f(1*i, 1*j);
                sf::Vector2f newGridPosition = gridPosition + blockOffset;
                sf::Vector2f pixelPosition({newGridPosition.x * 16.f, newGridPosition.y * 16.f});
                Tile tile = *TileManager::loadTile(tileData["tile"]);
                tile.setPosition(pixelPosition);
                if (tileData["skinOverride"] != -1){
                    tile.setTextureIndex(tileData["skinOverride"]);
                }
                else{
                    tile.setTextureIndex(file["globalSkinIndex"]);
                }
                level.tiles.emplace_back(tile);
            }
        }
    }
    std::cout << "Loaded level: \"" << name << "\"" << std::endl;
    return level;
}