//
// Created by jay on 09/01/2026.
//

#include "Player.h"

#include <cmath>
#include <iostream>
#include <random>

#include "AnimationManager.h"
#include "GameManager.h"
#include "Globals.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "Tile.h"

Player::Player() :
//@TODO store configurables in some .cfg file
animationSubManager(),
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
    texture = std::make_shared<sf::Texture>(TextureManager::loadTexture("mariosheet.png"));
    sprite = std::make_unique<sf::Sprite>(*texture);
    auto* anims = AnimationManager::loadAnimationFile("anim_player.json");
    animationSubManager.loadAnimations(anims);
}

sf::FloatRect Player::getBoundingBox() const{
    return physicsBox;
}

sf::Vector2f Player::getPosition() const{
    return position;
}

void Player::move(){
    moveX();
    moveY();
}

void Player::moveX(){
    velocity.x += walkAcceleration * Globals::getDeltaTime() * direction;

    if (direction == 0){
        velocity.x *= std::exp(-dampening * Globals::getDeltaTime());
        if (velocity.x < 1.f && velocity.x > -1.f){
            velocity.x = 0.f;
        }
    }

    velocity.x = std::clamp(velocity.x, -walkSpeed, walkSpeed);
    position.x += velocity.x * Globals::getDeltaTime();

    constexpr float skidThresh = 90.f;
    if ((velocity.x >= skidThresh && direction == -1) || (velocity.x <= -skidThresh && direction == 1)){
        isSkidding = true;
    }
    // We do this so that player keeps skidding until fully turned around
    else if (animationSubManager.getCurrentAnimationName() == "skid" && ((velocity.x > 0.f && direction == -1 ) || (velocity.x < 0.f && direction == 1))){
        isSkidding = true;
    }
    else{
        isSkidding = false;
    }
}

void Player::moveY(){
    velocity.y += gravity * Globals::getDeltaTime();
    position.y += velocity.y * Globals::getDeltaTime();
}

void Player::update() {
    if (!sprite){
        throw std::runtime_error("Player object uninitialised!");
    }
    onGround = false;
    move();
    updateAnimation();
    physicsBox.position = position;
}

void Player::updateAnimation(){
    if (isJumping){
        animationSubManager.playAnimation("jump");
    }
    else if (isSkidding){
        animationSubManager.playAnimation("skid");
    }
    else if(velocity.x != 0){
        animationSubManager.playAnimation("walk");
    }
    else{
        animationSubManager.playAnimation("idle");
    }

    const float absVelocityX = std::fabs(velocity.x);

    // Offsets the animationScale a little so that it looks better
    constexpr float animationScaleOffset = 0.4f;
    const float animationScale = (1.f - (absVelocityX / walkSpeed)) + animationScaleOffset;

    Animation* currentAnimation = animationSubManager.getCurrentAnimation();

    if (currentAnimation != nullptr) {
        if (direction != 0){
            // Have to multiply int direction by a float or it'll whine about narrowing conversions (it's literally a 1 and a -1 i don't understand why its being so stupid??)
            sprite->setScale({1.f * direction, 1.f});
        }
        if (sprite->getOrigin() != sprite->getLocalBounds().getCenter()){
            sprite->setOrigin(sprite->getLocalBounds().getCenter());
        }
        sprite->setPosition(physicsBox.position + sprite->getOrigin());
        sprite->setTextureRect(currentAnimation->getFrameRect());
        currentAnimation->setFrameDurationScale(14.f * animationScale / 60.f);
        currentAnimation->update();
    }
}

void Player::collide(const CollisionSide side, const sf::FloatRect overlap){
    collideX(side, overlap);
    collideY(side, overlap);
}

void Player::collideX(const CollisionSide side, const sf::FloatRect overlap){
    if (side == Left){
        velocity.x = 0;
        position.x -= overlap.size.x;
    }
    else if (side == Right){
        velocity.x = 0;
        position.x += overlap.size.x;
    }
}

void Player::collideY(const CollisionSide side, const sf::FloatRect overlap){
    if (side == Top){
        velocity.y = 0;
        position.y -= overlap.size.y;
        isJumping = false;
        onGround = true;
    }
    else if (side == Bottom){
        velocity.y = 0;
        position.y += overlap.size.y;
    }
}

void Player::handleInput() {
    const bool left = InputManager::isKeyPressed(sf::Keyboard::Key::A);
    const bool right = InputManager::isKeyPressed(sf::Keyboard::Key::D);

    if (InputManager::isLastKeyPressed(sf::Keyboard::Key::A)){
        direction = -1;
    }
    else if (InputManager::isLastKeyPressed(sf::Keyboard::Key::D)){
        direction = 1;
    }
    else if (left && !right){
        direction = -1;
    }
    else if (right && !left){
        direction = 1;
    }
    else if (!left && !right){
        direction = 0;
    }

    if (InputManager::isLastKeyPressed(sf::Keyboard::Key::Space) && onGround && !isJumping){
        onGround = false;
        isJumping = true;
        velocity.y -= jumpStrength;
    }
}

void Player::draw(sf::RenderTarget& target) const{
    target.draw(*sprite);
}