//
// Created by jay on 25/01/2026.
//

#include "TileManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "AnimationManager.h"
#include "GameManager.h"
#include "ManagerUtilities.h"
#include "TextureManager.h"

std::map<std::string, Tile> TileManager::tiles = {};
std::string TileManager::fullPath = "";

void TileManager::initialiseTiles(){
    fullPath = GameManager::getAssetPath() + "models/";

    const std::vector files(ManagerUtilities::findFiles(fullPath, {".json"}));

    for (const std::string& file : files){
        const Tile tile{initialiseTile(file)};
        const std::string tileName{std::filesystem::path(fullPath + file).stem().string()};
        std::cout << "Initialised tile: \"" << tileName << "\"" << std::endl;
        tiles.emplace(tileName, tile);
    }
}

Tile TileManager::initialiseTile(const std::string& path){
    const TileJson json = parseTile(fullPath + path);
    Tile tile;
    if (!json.animationFile.empty()){
        tile.setAnimated();
        const auto* animations{AnimationManager::loadAnimationFile(json.animationFile)};
        tile.addAnimations(animations);
        // @TODO default animation implementation
        tile.playAnimation("idleTile");
        tile.setFrameDurationScale(0.15f);
    }
    for (const auto& texture : json.textures){
        if (TextureManager::isTexture(texture)){
            tile.addTexture(TextureManager::loadTexture(texture));
        }
        else{
            throw std::runtime_error("Texture \"" + texture + "\" not found!");
        }
    }
    if (json.properties.isBreakable) {
        tile.setBreakable();
    }
    if (json.properties.isSolid) {
        tile.setSolid();
    }

    tile.setTextureIndex(0);
    return tile;
}

TileJson TileManager::parseTile(const std::string& filePath){
    std::ifstream modelFileData(filePath);
    nlohmann::json modelFile(nlohmann::json::parse(modelFileData));

    TileJson tile;

    if (!modelFile["animationFile"].empty()){
        tile.animationFile = modelFile["animationFile"].get<std::string>();
    }
    if (modelFile["properties"].empty()) {
        throw std::runtime_error("Tile has no properties!");
    }
    tile.properties = parseTileProperties(modelFile["properties"]);

    if (modelFile["textureFiles"].empty()){
        throw std::runtime_error("No textures referenced by tile!");
    }
    tile.textures = modelFile["textureFiles"].get<std::vector<std::string>>();

    return tile;
}

TileProperties TileManager::parseTileProperties(const nlohmann::basic_json<>& properties) {
    TileProperties tileProperties{};

    if (properties["isBreakable"].empty()) {
        throw std::runtime_error("Tile has no breakable property!");
    }
    tileProperties.isBreakable = properties["isBreakable"].get<bool>();

    std::cout << tileProperties.isBreakable << std::endl;

    if (properties["isSolid"].empty()) {
        throw std::runtime_error("Tile has no solid property!");
    }
    tileProperties.isSolid = properties["isSolid"].get<bool>();

    return tileProperties;
}

bool TileManager::isTile(const std::string& name){
    if (!isInitialised()){
        throw std::runtime_error("No tiles initialised!");
    }
    return tiles.contains(name);
}

Tile* TileManager::loadTile(const std::string& name){
    if (!isTile(name)){
        throw std::runtime_error("Could not find tile: \"" + name + "\"");
    }
    std::cout << "Loaded tile: \"" << name << "\"" << std::endl;
    return &tiles.at(name);
}

bool TileManager::isInitialised(){
    return ManagerUtilities::isInitialised(tiles);
}
