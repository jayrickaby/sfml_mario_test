//
// Created by jay on 09/01/2026.
//

#include "Player.h"

#include <cmath>
#include <iostream>

#include "constants.h"
#include "TextureManager.h"

Player::Player() :
texture(&TextureManager::loadTexture("assets/textures/mariosheet.png")),
sprite(*texture),
currentAnimation(nullptr),
onGround(false),
direction(0),
lastDirection(1),
walkSpeed(96.f),
walkAcceleration(128.f),
runSpeed(160.f),
runAcceleration(196.f),
dampening(7.5f),
gravity(175.f),
jumpStrength(125.f),
velocity({0,0}),
position({0,0}),
physicsBox(sf::FloatRect({16,16}, {0,0})) {
    Animation idleAnimation;
    idleAnimation.addFrame(sf::IntRect({0,0}, {16,16}));

    Animation walkAnimation;
    walkAnimation.addFrame(sf::IntRect({16,0}, {16,16}));
    walkAnimation.addFrame(sf::IntRect({32,0}, {16,16}));
    walkAnimation.addFrame(sf::IntRect({48,0}, {16,16}));

    Animation skidAnimation;
    skidAnimation.addFrame(sf::IntRect({64,0}, {16,16}));

    Animation jumpAnimation;
    jumpAnimation.addFrame(sf::IntRect({80,0}, {16,16}));

    animations["idle"] = idleAnimation;
    animations["walk"] = walkAnimation;
    animations["skid"] = skidAnimation;
    animations["jump"] = jumpAnimation;
}

void Player::update(float deltaTime) {
    handleInput();
    onGround = false;

    // Adjust origin once
    if (sprite.getOrigin() != sprite.getLocalBounds().getCenter()){
        sprite.setOrigin(sprite.getLocalBounds().getCenter());
    }

    float absVelocityX = std::fabs(velocity.x);

    if (direction == 0) {
        if (absVelocityX < 1.f){
            velocity.x = 0;
        }
        else {
            velocity.x *= exp(-dampening * deltaTime);
        }
    } else {
        // Needed for proper sprite flipping
        lastDirection = direction;
        velocity.x += walkSpeed * direction * deltaTime;
    }
    velocity.x = std::clamp(velocity.x, -walkSpeed, walkSpeed);
    position.x += velocity.x * deltaTime;

    velocity.y += gravity * deltaTime;

    position.y += velocity.y * deltaTime;

    const float groundPosition = SCREEN_HEIGHT - sprite.getLocalBounds().size.y;

    if (position.y >= groundPosition) {
        position.y = groundPosition;
        velocity.y = 0;
        onGround = true;
    }

    physicsBox.position = position;
    sprite.setPosition(physicsBox.position);

    absVelocityX = std::fabs(velocity.x);

    if (!onGround){
        setAnimation("jump");
    }
    else if (velocity.x != 0) {
        if ((direction == 1 && velocity.x > 0) || (direction == -1 && velocity.x < 0)){
            setAnimation("walk");
        }
        // Application of walk acceleration means that you will never actually achieve absVelocityX >= walkSpeed.
        // Subtracting walkAcceleration from the walkSpeed means a feasible value can be achieved
        else if (currentAnimationName != "skid" && absVelocityX >= walkSpeed - walkAcceleration){
            setAnimation("skid");
        }
    }
    else {
        setAnimation("idle");
    }

    // Offsets the animationScale a little so that it looks better
    const float animationScaleOffset = 0.4f;
    float animationScale = (1.f - (absVelocityX / walkSpeed)) + animationScaleOffset;

    // Higher animationScale = slower
    // Lower animationScale = faster
    currentAnimation->setFrameDurationScale(14.f * animationScale / 60.f);

    if (currentAnimation != nullptr) {
        sprite.setTextureRect(currentAnimation->getFrameRect());
        sprite.setScale({1.f * lastDirection,1.f});
        currentAnimation->update(deltaTime);
    }
}

void Player::handleInput() {
    direction = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);

    if (isKeyPressed(sf::Keyboard::Key::Space) && onGround){
        onGround = false;
        velocity.y -= jumpStrength;
    }
}

void Player::draw(sf::RenderTarget& target) {
    target.draw(sprite);
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