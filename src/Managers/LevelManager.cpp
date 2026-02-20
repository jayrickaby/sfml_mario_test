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

    if (data.contains("tileData")) {
        for (const auto& tile : data["tileData"]) {
            if (tile.empty()) {
                continue;
            }
            TileDataJson tileDataJson;
            if (tile.contains("tile")
                && tile["tile"].is_string()
                && !tile["tile"].empty()) {
                tileDataJson.tile = tile["tile"];
            }
            else {
                spdlog::warn("Tile is not specified! Skipping...");
                continue;
            }

            if (tile.contains("pos")
                && tile["pos"].is_array()
                && !tile["pos"].empty()) {
                tileDataJson.pos = ManagerUtilities::getVector2iFromJson(tile["pos"]);
            }
            else {
                spdlog::warn("Tile has no position specified! Skipping...");
                continue;
            }
            if (tile.contains("size")
                && tile["size"].is_array()
                && !tile["size"].empty()) {
                tileDataJson.size = ManagerUtilities::getVector2iFromJson(tile["size"]);
                }
            else {
                spdlog::warn("Tile has no size specified! Skipping");
                continue;
            }
            levelJson.tiles.emplace_back(tileDataJson);
        }
    }

    return levelJson;
}

Level LevelManager::createLevel(const LevelJson& levelJson) {
    Level level;
    for (const auto& tileData : levelJson.tiles) {
        for (int i = 0; i < tileData.size.x; i++) {
            for (int j = 0; j < tileData.size.y; j++) {
                sf::Vector2i newPosition{tileData.pos.x + i, tileData.pos.y + j};
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
