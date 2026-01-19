//
// Created by jay on 09/01/2026.
//

#include "Player.h"

#include <cmath>

#include "TextureManager.h"

Player::Player() :
texture(&TextureManager::loadTexture("assets/textures/mariosheet.png")),
sprite(*texture),
currentAnimation(nullptr),
direction(0),
lastDirection(1),
walkSpeed(96.f),
walkAcceleration(128.f),
runSpeed(160.f),
runAcceleration(196.f),
dampening(7.5f),
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

void Player::update(float deltaTime) {
    handleInput();
    sprite.setOrigin(sprite.getLocalBounds().getCenter());

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

    absVelocityX = std::fabs(velocity.x);

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