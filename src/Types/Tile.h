//
// Created by jay on 26/01/2026.
//

#ifndef SFML_MARIO_TEST_TILE_H
#define SFML_MARIO_TEST_TILE_H
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "../Managers/AnimationSubManager.h"

struct TileProperties {
    bool isSolid;
    bool isBreakable;
};

class Tile{
public:
    Tile();
    ~Tile() = default;

    void update();
    void draw(sf::RenderTarget& target) const;
    void playAnimation(const std::string& name);

    // Validator
    bool isAnimated() const;
    bool isBreakable() const;
    bool isSolid() const;

    // Getters
    sf::FloatRect getBoundingBox() const;

    // Setters
    void addAnimations(const std::map<std::string, Animation>* animations);
    void addTexture(const sf::Texture& texture);
    void setSolid();
    void setBreakable();
    void setAnimated();
    void setFrameDurationScale(const float& scale);
    void setPosition(const sf::Vector2i& position);
    void setTextureIndex(const int& index);
private:
    std::vector<const sf::Texture*> textures = {};
    sf::Sprite sprite;
    std::optional<AnimationSubManager> animationSubManager;
    TileProperties properties;
};
#endif //SFML_MARIO_TEST_TILE_H