//
// Created by jay on 20/02/2026.
//

#include "LevelManager.h"

#include <fstream>

#include "ManagerUtilities.h"
#include "TileManager.h"
#include "spdlog/spdlog.h"
#include "../Types/Level.h"


bool LevelManager::initialised;
std::filesystem::path LevelManager::fullPath;
std::vector<std::filesystem::path> LevelManager::levels;

void LevelManager::initialise() {
    fullPath = std::filesystem::path("assets/levels");
    spdlog::info("Initialising Level Manager...");
    levels = ManagerUtilities::getFilesFromPath(fullPath, {".json"});
    for (const auto& level : levels) {
        spdlog::info("Found level: {}", level.stem().string());
    }
    initialised = true;
    spdlog::info ("Initialised Level Manager!");
}

bool LevelManager::isLevel(const std::filesystem::path& path) {
    if (!isInitialised()) {
        spdlog::error("Tried to check if Level \"{}\" exists in uninitialised Level Manager!", path.string());
        throw std::runtime_error("Level Manager is uninitialised!");
    }
    return std::ranges::contains(levels, path);
}
bool LevelManager::isInitialised() {
    return initialised;
}

Level LevelManager::getLevel(const std::filesystem::path& path) {
    if (!isLevel(path)) {
        spdlog::error("Tried to get Level \"{}\" but it doesn't exist!", path.string());
        throw std::invalid_argument("Level doesn't exist!");
    }

    std::ifstream fileContents{(fullPath / path).string()};
    if (ManagerUtilities::isFileEmpty(fileContents)) {
        spdlog::critical("File: \"{}\" has no data!");
        throw std::invalid_argument("Level file has no data!");
    }
    nlohmann::basic_json<> data(nlohmann::json::parse(fileContents));

    return createLevel(parseLevelJson(data));
}

LevelJson LevelManager::parseLevelJson(const nlohmann::basic_json<>& data) {
    if (data.empty()) {
        spdlog::warn("Level has no data!");
        return {};
    }

    LevelJson levelJson;

    if (data.contains("levelProperties")) {
        LevelPropertiesJson levelProperties;
        const auto& properties(data["levelProperties"]);
        if (properties.contains("backgroundColour")
            && properties["backgroundColour"].is_array()
            && properties["backgroundColour"].size() == 3) {
            levelProperties.backgroundColour[0] = properties["backgroundColour"][0].get<int>();
            levelProperties.backgroundColour[1] = properties["backgroundColour"][1].get<int>();
            levelProperties.backgroundColour[2] = properties["backgroundColour"][2].get<int>();
        }
        else {
            spdlog::warn("No background colour provided! Using default.");
        }
        levelJson.properties = levelProperties;
    }

    if (data.contains("tileData")) {
        for (const auto& tile : data["tileData"]) {
            if (tile.empty()) {
                continue;
            }
            TileDataJson tileDataJson;
            if (tile.contains("tile")
                && tile["tile"].is_string()
                && !tile["tile"].empty()) {
                tileDataJson.tile = tile["tile"].get<std::string>();
            }
            else {
                spdlog::warn("Tile is not specified! Skipping tile...");
                continue;
            }

            if (tile.contains("pos")
                && tile["pos"].is_array()
                && tile["pos"].size() == 2) {
                tileDataJson.pos[0] = tile["pos"][0].get<int>();
                tileDataJson.pos[1] = tile["pos"][1].get<int>();
            }
            else {
                spdlog::warn("Tile has invalid position! Skipping tile...");
                continue;
            }
            if (tile.contains("size")
                && tile["size"].is_array()
                && tile["size"].size() == 2) {
                tileDataJson.size[0] = tile["size"][0].get<int>();
                tileDataJson.size[1] = tile["size"][1].get<int>();
                }
            else {
                spdlog::warn("Tile has invalid size! Skipping tile...");
                continue;
            }
            levelJson.tiles.emplace_back(tileDataJson);
        }
    }

    return levelJson;
}

Level LevelManager::createLevel(const LevelJson& levelJson) {
    Level level;
    level.properties.backgroundColour.r = levelJson.properties.backgroundColour[0];
    level.properties.backgroundColour.g = levelJson.properties.backgroundColour[1];
    level.properties.backgroundColour.b = levelJson.properties.backgroundColour[2];

    for (const auto& tileData : levelJson.tiles) {
        for (int i = 0; i < tileData.size[0]; i++) {
            for (int j = 0; j < tileData.size[1]; j++) {
                sf::Vector2i newPosition{tileData.pos[0] + i, tileData.pos[1] + j};
                sf::Vector2i pixelPosition{newPosition.x * 16, newPosition.y * 16};

                Tile tile = TileManager::getTile(tileData.tile);
                tile.setPosition(pixelPosition);

                std::pair<int, int> gridPos;
                gridPos.first = newPosition.x;
                gridPos.second = newPosition.y;

                level.tiles.emplace(gridPos, tile);
            }
        }
    }
    return level;
}
