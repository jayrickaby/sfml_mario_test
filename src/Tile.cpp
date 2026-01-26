//
// Created by jay on 26/01/2026.
//

#include "Tile.h"

void Tile::draw(sf::RenderTarget& target) const{
    if (sprite.has_value()){
        target.draw(*sprite);
    }
}

void Tile::addTexture(const sf::Texture& texture){
    textures.emplace_back(&texture);
}

void Tile::setPosition(const sf::Vector2f& position){
    if (sprite.has_value()){
        sprite->setPosition(position);
    }
}

void Tile::setTextureIndex(const int index){
    sprite.emplace(*textures[index]);
}
