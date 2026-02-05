//
// Created by jay on 09/01/2026.
//

#include "Player.h"

#include <cmath>
#include <iostream>
#include <random>

#include "AnimationManager.h"
#include "constants.h"
#include "GameManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "Tile.h"

Player::Player() :
//@TODO store configurables in some .cfg file
currentAnimation(nullptr),
onGround(false),
isJumping(false),
isSkidding(false),
direction(0),
walkSpeed(96.f),
walkAcceleration(128.f),
runSpeed(160.f),
runAcceleration(196.f),
dampening(8.5f),
gravity(450.f),
jumpStrength(250.f),
velocity({0,0}),
position({0,0}),
physicsBox(sf::FloatRect({16,16}, {16,16})) {}

// Player tries to grab textures before they even exist. This allows that to be seperately handled
void Player::initialisePlayer(){
    texture = std::make_shared<sf::Texture>(TextureManager::loadTexture("assets/textures/mariosheet.png"));
    sprite = std::make_unique<sf::Sprite>(*texture);
    animations = *AnimationManager::loadAnimation("assets/animations/anim_player.json");
}

void Player::update(float deltaTime) {
    if (!sprite){
        throw std::runtime_error("Player object unitialised!");
    }
    handleInput();
    onGround = false;

    velocity.x += walkAcceleration * deltaTime * direction;

    if (direction == 0){
        velocity.x *= std::exp(-dampening * deltaTime);
        if (velocity.x < 1.f && velocity.x > -1.f){
            velocity.x = 0.f;
        }
    }

    velocity.x = std::clamp(velocity.x, -walkSpeed, walkSpeed);
    position.x += velocity.x * deltaTime;

    constexpr float skidThresh = 90.f;
    if ((velocity.x >= skidThresh && direction == -1) || (velocity.x <= -skidThresh && direction == 1)){
        isSkidding = true;
    }
    // We do this so that player keeps skidding until fully turned around
    else if ((currentAnimationName == "skid") && ((velocity.x > 0.f && direction == -1 ) || (velocity.x < 0.f && direction == 1))){
        isSkidding = true;
    }
    else{
        isSkidding = false;
    }

    velocity.y += gravity * deltaTime;
    position.y += velocity.y * deltaTime;

    if (isJumping){
        setAnimation("jump");
    }
    else if (isSkidding){
        setAnimation("skid");
    }
    else if(velocity.x != 0){
        setAnimation("walk");
    }
    else{
        setAnimation("idle");
    }

    const float absVelocityX = std::fabs(velocity.x);

    // Offsets the animationScale a little so that it looks better
    constexpr float animationScaleOffset = 0.4f;
    float animationScale = (1.f - (absVelocityX / walkSpeed)) + animationScaleOffset;

    if (currentAnimation != nullptr) {
        if (direction != 0){
            // Have to multiply int direction by a float or it'll whine about narrowing conversions
            sprite->setScale({1.f * direction, 1.f});
        }
        if (sprite->getOrigin() != sprite->getLocalBounds().getCenter()){
            sprite->setOrigin(sprite->getLocalBounds().getCenter());
        }
        sprite->setPosition(physicsBox.position + sprite->getOrigin());
        sprite->setTextureRect(currentAnimation->getFrameRect());
        currentAnimation->setFrameDurationScale(14.f * animationScale / 60.f);
        currentAnimation->update(deltaTime);
    }
    physicsBox.position = position;
}

void Player::collide(const sf::FloatRect& collisionBox){
    auto intersection = collisionBox.findIntersection(physicsBox);
    if (!intersection.has_value()){
        return;
    }
    sf::FloatRect overlap = intersection.value();
    if (overlap.size.y > overlap.size.x){
        if (velocity.x > 0){
            position.x -= overlap.size.x;
        }
        else if (velocity.x < 0){
            position.x += overlap.size.x;
        }
        velocity.x = 0;
    }
    else if (overlap.size.x > overlap.size.y){
        if (velocity.y > 0){
            position.y -= overlap.size.y;
            onGround = true;
            isJumping = false;
        }
        else if (velocity.y < 0){
            position.y += overlap.size.y;
        }
        velocity.y = 0;
    }
}

void Player::handleInput() {
    if (InputManager::isLastKeyPressed(sf::Keyboard::Key::A)){
        direction = -1;
    }
    else if (InputManager::isLastKeyPressed(sf::Keyboard::Key::D)){
        direction = 1;
    }
    else if (InputManager::isLastKeyPressed(sf::Keyboard::Key::Unknown) && !(InputManager::isKeyPressed(sf::Keyboard::Key::A) || InputManager::isKeyPressed(sf::Keyboard::Key::D))){
        direction = 0;
    }
    if (InputManager::isKeyPressed(sf::Keyboard::Key::A) && !InputManager::isKeyPressed(sf::Keyboard::Key::D) && direction == 1){
        direction = -1;
    }
    else if (InputManager::isKeyPressed(sf::Keyboard::Key::D) && !InputManager::isKeyPressed(sf::Keyboard::Key::A) && direction == -1){
        direction = 1;
    }

    if (InputManager::isLastKeyPressed(sf::Keyboard::Key::Space) && onGround && !isJumping){
        onGround = false;
        isJumping = true;
        velocity.y -= jumpStrength;
    }
}

void Player::draw(sf::RenderTarget& target) {
    target.draw(*sprite);
}

void Player::setAnimation(const std::string& name) {
    if (currentAnimationName == name){
      return;
    }
    if (currentAnimation != nullptr) {
        currentAnimation->reset();
    }

    if (animations.contains(name)) {
        currentAnimationName = name;
        currentAnimation = &animations.at(currentAnimationName);
    }
    else {
        throw std::runtime_error("Animation \"" + name + "\" doesn't exist!");
    }
}