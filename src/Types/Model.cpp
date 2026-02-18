//
// Created by jay on 18/02/2026.
//

#include "Model.h"

#include <algorithm>
#include <spdlog/spdlog.h>

#include "../Managers/TextureManager.h"
#include "SFML/Graphics/RenderTarget.hpp"

Model::Model() :
    sprite(*TextureManager::getAtlas())
{}

void Model::update() {
    // So it default to atlas rect's proportions
    sf::IntRect rect{textures[currentTextureName]->rect};

    if (animations) {
        animations->update();

        // So it adjust proportions based on animation frame (if applicable)
        rect.position += animations->getCurrentFrame().position;
        rect.size = animations->getCurrentFrame().size;
    }

    sprite.setTextureRect(rect);
}

void Model::draw(sf::RenderTarget& target) const {
    target.draw(sprite);
}

bool Model::isTexture(const std::filesystem::path& path) const {
    return textures.contains(path);
}

std::filesystem::path Model::getDefaultTextureName() const {
    return defaultTextureName;
}

void Model::addTexture(const std::filesystem::path& path, Texture* texture) {
    if (isTexture(path)) {
        spdlog::warn("Tried adding texture \"{}\" but it already exists in list!");
        throw std::invalid_argument("Texture already exists!");
    }
    textures.emplace(path, texture);
}
void Model::setAnimationSet(const AnimationSet& animationSet) {
    animations = animationSet;
}
void Model::setDefaultTexture(const std::filesystem::path& path) {
    if (isTexture(path)) {
        defaultTextureName = path;
        return;
    }
    if (!path.empty()) {
        spdlog::warn("Tried to set default texture to \"{}\" which doesn't exist. Defaulting to first in list!", path.string());
    }
    else {
        spdlog::warn("No default texture. Defaulting to first in list!");
    }
    defaultTextureName = textures.begin()->first;
}
void Model::setTexture(const std::filesystem::path& path) {
    if (!isTexture(path)) {
        spdlog::warn("Tried to set texture to \"{}\" which doesn't exist! Setting to default");
        currentTextureName = defaultTextureName;
        return;
    }
    currentTextureName = path;

    sprite.setTextureRect(textures[currentTextureName]->rect);
}