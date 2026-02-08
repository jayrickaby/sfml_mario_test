//
// Created by jay on 08/02/2026.
//

#ifndef SFML_MARIO_TEST_ANIMATIONSUBMANAGER_H
#define SFML_MARIO_TEST_ANIMATIONSUBMANAGER_H
#include <map>
#include <string>

#include "Animation.h"


class AnimationSubManager{
public:
    AnimationSubManager() = default;
    ~AnimationSubManager() = default;

    void loadAnimations(const std::map<std::string, Animation> *givenAnimations);

    // Setters
    void playAnimation(const std::string& name);

    // Getters
    Animation* getCurrentAnimation() const;
    std::string getCurrentAnimationName() const;
private:
    std::map<std::string, Animation> animations;
    Animation* currentAnimation;
};


#endif //SFML_MARIO_TEST_ANIMATIONSUBMANAGER_H