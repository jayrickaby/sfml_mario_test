//
// Created by jay on 25/01/2026.
//

#ifndef SFML_MARIO_TEST_TILEMANAGER_H
#define SFML_MARIO_TEST_TILEMANAGER_H
#include <string>

#include "nlohmann/json.hpp"


class Tile;
struct TileJson;

class TileManager {
public:
    TileManager() = default;
    ~TileManager() = default;

    static void initialise();

    // Validators
    static bool isTile(const std::string& name);
    static bool isInitialised();

    // Getters
    static Tile getTile(const std::string& name);
    static std::map<std::string, Tile> getTiles();

private:
    static TileJson parseTileJson(const nlohmann::basic_json<>& data);

    static Tile createTile(const TileJson& tileJson);

    static bool initialised;
    static std::filesystem::path fullPath;
    static std::map<std::string, Tile> tiles;
};


#endif //SFML_MARIO_TEST_TILEMANAGER_H