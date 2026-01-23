//
// Created by jay on 09/01/2026.
//

#include "Player.h"

#include <cmath>

#include "AnimationManager.h"
#include "constants.h"
#include "TextureManager.h"

Player::Player() :
texture(&TextureManager::loadTexture("assets/textures/mariosheet.png")),
animations(AnimationManager::loadAnimation("assets/animations/anim_player.json")),
sprite(*texture),
currentAnimation(nullptr),
onGround(false),
isJumping(false),
isSkidding(false),
direction(0),
lastDirection(1),
walkSpeed(96.f),
walkAcceleration(128.f),
runSpeed(160.f),
runAcceleration(196.f),
dampening(7.5f),
gravity(400.f),
jumpStrength(200.f),
velocity({0,0}),
position({0,0}),
physicsBox(sf::FloatRect({16,16}, {0,0})) {}

void Player::update(float deltaTime) {
    handleInput();
    onGround = false;

    // Adjust origin once
    if (sprite.getOrigin() != sprite.getLocalBounds().getCenter()){
        sprite.setOrigin(sprite.getLocalBounds().getCenter());
    }

    float absVelocityX = std::fabs(velocity.x);

    // @TODO fix skidding nitwit
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
        velocity.x += walkAcceleration * direction * deltaTime;
        velocity.x = std::clamp(velocity.x, -walkSpeed, walkSpeed);

        const float skiddingThreshold = walkSpeed - walkAcceleration * deltaTime;

        if ((velocity.x >= (skiddingThreshold) && direction == -1)
            || velocity.x <= -(skiddingThreshold) && direction == 1 ){
            isSkidding = true;
        }
        else if (isSkidding && (absVelocityX < EPSILON ||
         (velocity.x > 0 && direction == 1) ||
         (velocity.x < 0 && direction == -1))) {
            isSkidding = false;
         }

    }

    position.x += velocity.x * deltaTime;

    velocity.y += gravity * deltaTime;

    position.y += velocity.y * deltaTime;

    constexpr float groundPosition = SCREEN_HEIGHT - 16.f;

    if (position.y >= groundPosition) {
        position.y = groundPosition;
        velocity.y = 0;
        onGround = true;
        isJumping = false;
    }

    physicsBox.position = position;
    sprite.setPosition(physicsBox.position - sprite.getOrigin());

    absVelocityX = std::fabs(velocity.x);


    if (!onGround && isJumping){
        setAnimation("jump");
    } else if (isSkidding){
        setAnimation("skid");
    } else if ((velocity.x < 0 && direction == -1) || velocity.x > 0 && direction == 1){
        setAnimation("walk");
    } else{
        setAnimation("idle");
    }

    // Offsets the animationScale a little so that it looks better
    constexpr float animationScaleOffset = 0.4f;
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
        isJumping = true;
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