//
// Created by jay on 26/01/2026.
//

#ifndef SFML_MARIO_TEST_TILE_H
#define SFML_MARIO_TEST_TILE_H
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "../Managers/AnimationSubManager.h"

class Tile{
public:
    Tile();
    ~Tile() = default;

    void update();
    void draw(sf::RenderTarget& target) const;

    // Validator
    bool isAnimated() const;

    // Getters
    sf::FloatRect getBoundingBox() const;

    // Setters
    void addAnimations(const std::map<std::string, Animation>* animations);
    void addTexture(const sf::Texture& texture);

    void playAnimation(const std::string& name);
    void setAnimated();
    void setFrameDurationScale(const float& scale);
    void setPosition(const sf::Vector2f& position);
    void setTextureIndex(int index);
private:
    std::vector<const sf::Texture*> textures = {};
    sf::Sprite sprite;
    std::optional<AnimationSubManager> animationSubManager;
};
#endif //SFML_MARIO_TEST_TILE_H