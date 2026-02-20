//
// Created by jay on 18/02/2026.
//

#ifndef SFML_MARIO_TEST_MODEL_H
#define SFML_MARIO_TEST_MODEL_H
#include <filesystem>
#include <vector>

#include "AnimationSet.h"
#include "Texture.h"
#include "SFML/Graphics/Sprite.hpp"

struct ModelJson {
    std::filesystem::path animationFile;
    std::vector<std::filesystem::path> textureFiles;
    std::filesystem::path defaultTexture;
};

class Model {
public:
    Model();

    void update();
    void draw(sf::RenderTarget& target) const;

    // Validators
    bool isTexture(const std::filesystem::path& path) const;

    // Getters
    std::filesystem::path getDefaultTextureName() const;
    sf::Sprite* getSprite();

    // Setters
    void addTexture(const std::filesystem::path& path, Texture* texture);
    void setAnimationSet(const AnimationSet& animationSet);
    void setDefaultTexture(const std::filesystem::path& path);
    void setTexture(const std::filesystem::path& path);

private:
    bool isSpriteInitialised() const;
    void initialiseSprite();

    std::optional<AnimationSet> animations;
    std::map<std::filesystem::path, Texture*> textures;
    sf::Sprite sprite;

    std::filesystem::path currentTextureName;
    std::filesystem::path defaultTextureName;
};

#endif //SFML_MARIO_TEST_MODEL_H