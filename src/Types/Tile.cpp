//
// Created by jay on 26/01/2026.
//

#include "Tile.h"

#include <iostream>

#include "../Managers/TextureManager.h"

Tile::Tile():
sprite(TextureManager::loadTexture("tiles/missing.png"))
{}

void Tile::update(){
    auto* anim = animationSubManager->getCurrentAnimation();
    sprite.setTextureRect(anim->getFrameRect());
    anim->update();
}

void Tile::draw(sf::RenderTarget& target) const{
    target.draw(sprite);
}

sf::FloatRect Tile::getBoundingBox() const{
    return sprite.getGlobalBounds();
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

bool Tile::isAnimated() const{
    return animationSubManager.has_value();
}

void Tile::setAnimated(){
    animationSubManager = AnimationSubManager();
}

void Tile::addAnimations(const std::map<std::string, Animation>* animations){
    animationSubManager->loadAnimations(animations);
}

void Tile::playAnimation(const std::string& name){
    animationSubManager->playAnimation(name);
}

void Tile::setFrameDurationScale(const float& scale){
    animationSubManager->getCurrentAnimation()->setFrameDurationScale(scale);
}
