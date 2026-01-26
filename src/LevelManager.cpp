//
// Created by jay on 26/01/2026.
//

#include "LevelManager.h"

#include "TileManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::vector<std::string> LevelManager::levels = {};
std::string LevelManager::levelDirectory;

void LevelManager::initialiseLevels(const std::string& path){
    levelDirectory = path;
    if (!std::filesystem::is_directory(path)){
        throw std::runtime_error("Could not find directory: \"" + path + "\"");
    }
    if (!TileManager::isInitialised()){
        throw std::runtime_error("Cannot initialise levels before initialising tiles!");
    }
    const std::filesystem::path directory{path};

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
    return level;
}
