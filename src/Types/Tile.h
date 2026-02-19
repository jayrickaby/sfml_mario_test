//
// Created by jay on 19/02/2026.
//

#ifndef SFML_MARIO_TEST_TILE_H
#define SFML_MARIO_TEST_TILE_H
#include <filesystem>

#include "Model.h"

struct TileJson {
    std::filesystem::path modelFile;
};

class Tile {
public:
    Tile() = default;
    ~Tile() = default;

    void update();
    void draw(sf::RenderTarget& target) const;

    void setModelFile(const std::filesystem::path& path);

private:
    Model model;
    sf::Vector2f position;
};


#endif //SFML_MARIO_TEST_TILE_H