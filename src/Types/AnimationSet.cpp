//
// Created by jay on 15/02/2026.
//

#include <spdlog/spdlog.h>
#include "AnimationSet.h"
#include "Animation.h"
#include "Frame.h"
#include "../Globals.h"

void AnimationSet::update() {
    if (currentAnimationName.empty()) {
        currentAnimationName = defaultAnimationName;
    }
    Animation* currentAnimation = &animations[currentAnimationName];

    const Frame& currentFrame = currentAnimation->frames[currentAnimation->frameIndex];

    currentAnimation->frameTimer += Globals::getDeltaTime() * currentAnimation->speed;
    if (currentAnimation->frameTimer >= currentFrame.duration) {
        // In case of lagspike, this corrects for overshoot of time
        currentAnimation->frameTimer -= currentFrame.duration;

        currentAnimation->frameIndex = (currentAnimation->frameIndex + 1) % currentAnimation->frames.size();
    }
}
void AnimationSet::reset() {
    Animation& currentAnimation = animations[currentAnimationName];
    currentAnimation.frameIndex = 0;
    currentAnimation.frameTimer = 0;
    currentAnimation.speed = currentAnimation.defaultSpeed;
}

bool AnimationSet::isAnimation(const std::string& name) const {
    return animations.contains(name);
}

std::string AnimationSet::getCurrentAnimationName() const {
    return currentAnimationName;
}
sf::IntRect AnimationSet::getCurrentFrame() {
    if (currentAnimationName.empty()) {
        currentAnimationName = defaultAnimationName;
    }
    const Animation& currentAnimation = animations.at(currentAnimationName);
    return currentAnimation.frames[currentAnimation.frameIndex].rect;
}

void AnimationSet::add(const std::string& name, const Animation& animation) {
    if (!animations.contains(name)) {
        animations.emplace(name, animation);
        return;
    }
    spdlog::warn("Tried adding animation: \"" + name + "\", but it already exists!");
}

void AnimationSet::play(const std::string& name) {
    if (!isAnimation(name)) {
        spdlog::error("Tried to play animation: \"" + name + "\" but it doesn't exist!");
        throw std::invalid_argument("Animation doesn't exist!");
    }

    if (currentAnimationName != name) {
        // So old animation is left how it was found
        reset();
        currentAnimationName = name;
    }
}

void AnimationSet::setDefaultAnimation(const std::string& name) {
    if (animations.contains(name)) {
        defaultAnimationName = name;
        return;
    }
    spdlog::warn("Tried setting non-existent animation: \"" + name + "\" as default animation.");
}
