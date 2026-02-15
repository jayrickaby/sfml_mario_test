//
// Created by jay on 25/01/2026.
//

#ifndef SFML_MARIO_TEST_TILEMANAGER_H
#define SFML_MARIO_TEST_TILEMANAGER_H
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "../Types/Tile.h"

struct TileJson{
    std::string animationFile;
    std::vector<std::string> textures;
};

class TileManager{
public:
    TileManager() = default;
    ~TileManager() = default;

    static void initialiseTiles();
    static TileJson parseTile(const std::string& filePath);
    static Tile initialiseTile(const std::string& path);
    static Tile* loadTile(const std::string& name);
    // Validators
    static bool isTile(const std::string& name);
    static bool isInitialised();


private:
    static std::map<std::string, Tile> tiles;
    static std::string fullPath;
};


#endif //SFML_MARIO_TEST_TILEMANAGER_H