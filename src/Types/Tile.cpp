//
// Created by jay on 19/02/2026.
//

#include "Tile.h"

#include "../Managers/ModelManager.h"
#include "spdlog/spdlog.h"

void Tile::update() {
    model.update();
}

void Tile::draw(sf::RenderTarget& target) const {
    model.draw(target);
}

void Tile::setModelFile(const std::filesystem::path& path) {
    if (!ModelManager::isModel(path)) {
        spdlog::error("Model file is not valid!");
        throw std::invalid_argument("Model file is not valid!");
    }
    model = ModelManager::getModel(path);
}
