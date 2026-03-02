//
// Created by jay on 20/02/2026.
//

#ifndef SFML_MARIO_TEST_LEVEL_H
#define SFML_MARIO_TEST_LEVEL_H
#include <map>
#include <SFML/Graphics.hpp>
#include "Tile.h"


struct TileDataJson {
    std::string tile;
    int pos[2];
    int size[2];
};

struct LevelPropertiesJson{
    int backgroundColour[3];
};

struct LevelJson {
    std::vector<TileDataJson> tiles;
    LevelPropertiesJson properties;
};

struct LevelProperties {
    sf::Color backgroundColour{0,0,0,255};
};

struct Level {
    std::map<std::pair<int, int>, Tile> tiles;
    LevelProperties properties;
};

#endif //SFML_MARIO_TEST_LEVEL_H