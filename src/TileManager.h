//
// Created by jay on 25/01/2026.
//

#ifndef SFML_MARIO_TEST_TILEMANAGER_H
#define SFML_MARIO_TEST_TILEMANAGER_H
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "Tile.h"

class TileManager{
public:
    TileManager() = default;
    ~TileManager() = default;

    static void initialiseTiles();
    static Tile initialiseTile(std::string& path);
    static bool isTile(const std::string& name);
    static bool isInitialised();
    static Tile* loadTile(const std::string& name);

private:
    static std::map<std::string, Tile> tiles;
    static std::string fullPath;
};


#endif //SFML_MARIO_TEST_TILEMANAGER_H