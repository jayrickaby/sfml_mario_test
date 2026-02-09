//
// Created by jay on 08/02/2026.
//

#include "AnimationSubManager.h"

void AnimationSubManager::playAnimation(const std::string& name){
    if (currentAnimationName == name){
        return;
    }

    if (!animations.contains(name)) {
        throw std::runtime_error("Animation \"" + name + "\" doesn't exist!");
    }

    if (!currentAnimationName.empty()){
        animations.at(currentAnimationName).reset();
    }
    currentAnimationName = name;
}

void AnimationSubManager::loadAnimations(const std::map<std::string, Animation> *givenAnimations){
    animations = *givenAnimations;
}

Animation* AnimationSubManager::getCurrentAnimation() {
    if (currentAnimationName.empty()){
        return nullptr;
    }
    return &animations.at(currentAnimationName);
}

std::string AnimationSubManager::getCurrentAnimationName() const{
    return currentAnimationName;
}
