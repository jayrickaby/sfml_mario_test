//
// Created by jay on 08/02/2026.
//

#include "AnimationSubManager.h"

void AnimationSubManager::playAnimation(const std::string& name){
    if (getCurrentAnimationName() == name){
        return;
    }
    if (currentAnimation != nullptr) {
        currentAnimation->reset();
    }

    if (animations.contains(name)) {
        currentAnimation = &animations.at(name);
    }
    else {
        throw std::runtime_error("Animation \"" + name + "\" doesn't exist!");
    }
}

void AnimationSubManager::loadAnimations(const std::map<std::string, Animation> *givenAnimations){
    animations = *givenAnimations;
}

Animation* AnimationSubManager::getCurrentAnimation() const{
    return currentAnimation;
}

std::string AnimationSubManager::getCurrentAnimationName() const{
    if (currentAnimation != nullptr){
        return currentAnimation->getName();
    }
    return "";
}
