//
// Created by jay on 25/01/2026.
//

#ifndef SFML_MARIO_TEST_TILEMANAGER_H
#define SFML_MARIO_TEST_TILEMANAGER_H
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Tile{
public:
    Tile() = default;
    ~Tile() = default;

    void addTexture(const sf::Texture& texture);
    void setPosition(const sf::Vector2f& position);
    void setTextureIndex(const int index);
    void draw(sf::RenderTarget& target) const;
private:
    std::vector<const sf::Texture*> textures = {};
    std::optional<sf::Sprite> sprite;
};

class TileManager{
public:
    TileManager() = default;
    ~TileManager() = default;

    static void initialiseTiles(const std::string& path);
    static bool isTile(const std::string& name);
    static bool isInitialised();
    static Tile* loadTile(const std::string& name);

private:
    static std::map<std::string, Tile> tiles;
};


#endif //SFML_MARIO_TEST_TILEMANAGER_H