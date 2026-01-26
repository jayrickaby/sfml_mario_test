//
// Created by jay on 26/01/2026.
//

#include "Tile.h"

#include "TextureManager.h"

Tile::Tile():
sprite(TextureManager::loadTexture("assets/textures/tiles/missing.png"))
{}

void Tile::draw(sf::RenderTarget& target) const{
    target.draw(sprite);
}

void Tile::addTexture(const sf::Texture& texture){
    textures.emplace_back(&texture);
}

void Tile::setPosition(const sf::Vector2f& position){
    sprite.setPosition(position);
}

void Tile::setTextureIndex(const int index){
    sprite.setTexture(*textures[index]);
}
