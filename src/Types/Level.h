//
// Created by jay on 20/02/2026.
//

#ifndef SFML_MARIO_TEST_LEVEL_H
#define SFML_MARIO_TEST_LEVEL_H
#include <map>

#include "Tile.h"

struct TileDataJson {
    std::string tile;
    sf::Vector2i pos;
    sf::Vector2i size;
};

struct LevelJson {
    std::vector<TileDataJson> tiles;
};

struct Level {
    std::map<std::pair<int, int>, Tile> tiles;
};

#endif //SFML_MARIO_TEST_LEVEL_H