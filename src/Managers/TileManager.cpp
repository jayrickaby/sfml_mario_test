//
// Created by jay on 25/01/2026.
//

#include <fstream>
#include "TileManager.h"
#include "../Types/Tile.h"

#include "ManagerUtilities.h"
#include "spdlog/spdlog.h"

bool TileManager::initialised = false;
std::filesystem::path TileManager::fullPath;
std::map<std::string, Tile> TileManager::tiles;

void TileManager::initialise() {
    fullPath = std::filesystem::path("assets/tiles");
    spdlog::info("Initialising Tile Manager!");
    const auto& files = ManagerUtilities::getFilesFromPath(fullPath, {".json"});
    for (const auto& file : files) {
        spdlog::info("Found tile: {}", file.string());

        std::ifstream fileContents{(fullPath / file).string()};
        if (ManagerUtilities::isFileEmpty(fileContents)) {
            spdlog::critical("File: \"{}\" has no data!");
            throw std::invalid_argument("Model file has no data!");
        }
        nlohmann::basic_json<> data(nlohmann::json::parse(fileContents));

        tiles.emplace(file.stem(), createTile(parseTileJson(data)));
    }

    initialised = true;
    spdlog::info("Initialised Tile Manager!");
}

bool TileManager::isTile(const std::string& name) {
    if (!isInitialised()) {
        spdlog::error("Tried to check if Tile \"{}\" exists in uninitialised Tile Manager!", name);
        throw std::runtime_error("Tile Manager is uninitialised!");
    }
    return tiles.contains(name);
}

bool TileManager::isInitialised() {
    return initialised;
}

Tile TileManager::getTile(const std::string& name) {
    if (!isTile(name)) {
        spdlog::error("Tried to get Tile \"{}\" but it doesn't exist!", name);
        throw std::invalid_argument("Tile doesn't exist!");
    }
    return tiles[name];
}

TileJson TileManager::parseTileJson(const nlohmann::basic_json<>& data) {
    if (data.empty()) {
        spdlog::critical("Tile has no data!");
        throw std::invalid_argument("Tile has no data!");
    }

    TileJson tileJson;

    // Model File
    if (data.contains("modelFile")) {
        if (!data["modelFile"].is_string()
            || data["modelFile"].empty()) {
            spdlog::critical("Invalid model file path!");
            throw std::invalid_argument("Invalid model file path!");
        }
        tileJson.modelFile = data["modelFile"].get<std::string>();
    }

    return tileJson;
}

Tile TileManager::createTile(const TileJson& tileJson) {
    Tile tile;

    tile.setModelFile(tileJson.modelFile);

    return tile;
}
