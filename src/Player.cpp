//
// Created by jay on 09/01/2026.
//

#include "Player.h"

#include <cmath>
#include <iostream>

#include "TextureManager.h"

Player::Player() :
texture(&TextureManager::loadTexture("assets/textures/mariosheet.png")),
sprite(*texture),
direction(0),
lastDirection(1),
walkSpeed(93.75f),
walkAcceleration(133.59375f),
runSpeed(153.75f),
runAcceleration(196.875f),
releaseDeceleration(182.8125f),
gravity(900.f),
jumpStrength(300.f),
velocity({0,0}),
position({0,0}),
physicsBox(sf::FloatRect({16,16}, {0,0})) {
    Animation idleAnimation;
    idleAnimation.addFrame(sf::IntRect({0,0}, {16,16}));

    Animation walkAnimation;
    walkAnimation.addFrame(sf::IntRect({16,0}, {16,16}));
    walkAnimation.addFrame(sf::IntRect({32,0}, {16,16}));
    walkAnimation.addFrame(sf::IntRect({48,0}, {16,16}));

    animations["idle"] = idleAnimation;
    animations["walk"] = walkAnimation;

}

void Player::handleInput() {
    direction = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
}

void Player::update(float deltaTime) {
    handleInput();
    //@TODO Fix Origin Issue
    std::cout << sprite.getOrigin().x << " " << sprite.getOrigin().y << std::endl;

    if (direction == 0) {
        if (velocity.x > 0) {
            velocity.x -= releaseDeceleration * deltaTime;
            if (velocity.x < 0) {
                velocity.x = 0;
            }
        }
        else if (velocity.x < 0) {
            velocity.x += releaseDeceleration * deltaTime;
            if (velocity.x > 0) {
                velocity.x = 0;
            }
        }
    } else {
        lastDirection = direction;
        velocity.x += walkSpeed * direction * deltaTime;
    }
    velocity.x = std::clamp(velocity.x, -walkSpeed, walkSpeed);
    position.x += velocity.x * deltaTime;

    velocity.y += gravity * deltaTime;

    if (position.y >= 128 - 16) {
        position.y = 128 - 16;
        velocity.y = 0;
    }

    position.y += velocity.y * deltaTime;

    physicsBox.position = position;
    sprite.setPosition(physicsBox.position);

    if (velocity.x != 0) {
        setAnimation("walk");
    }
    else {
        setAnimation("idle");
    }
    float absVelocityX = std::fabs(velocity.x);

    if (absVelocityX >= 105.f) {
        currentAnimation->setFrameDurationScale(2.f/60.f);
    }
    else if (absVelocityX >= 52.5f) {
        currentAnimation->setFrameDurationScale(4.f/60.f);
    }
    else {
        currentAnimation->setFrameDurationScale(7.f/60.f);
    }

    if (currentAnimation != nullptr) {
        sprite.setTextureRect(currentAnimation->getFrameRect());
        sprite.setScale({lastDirection,1.f});
        currentAnimation->update(deltaTime);
    }
}

void Player::draw(sf::RenderTarget& target) {
    target.draw(sprite);
}

void Player::setAnimation(std::string name) {
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