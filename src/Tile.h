//
// Created by jay on 26/01/2026.
//

#ifndef SFML_MARIO_TEST_TILE_H
#define SFML_MARIO_TEST_TILE_H
#include <SFML/Graphics.hpp>

class Tile{
public:
    Tile();
    ~Tile() = default;

    void addTexture(const sf::Texture& texture);
    void setPosition(const sf::Vector2f& position);
    void setTextureIndex(int index);
    void draw(sf::RenderTarget& target) const;
private:
    std::vector<const sf::Texture*> textures = {};
    sf::Sprite sprite;
};

#endif //SFML_MARIO_TEST_TILE_H