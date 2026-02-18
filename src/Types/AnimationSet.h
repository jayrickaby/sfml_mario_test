//
// Created by jay on 15/02/2026.
//

#ifndef SFML_MARIO_TEST_ANIMATIONSET_H
#define SFML_MARIO_TEST_ANIMATIONSET_H
#include <map>
#include <string>
#include <vector>

#include "SFML/Graphics/Rect.hpp"
#include "Animation.h"

struct AnimationSetJson {
    std::vector<AnimationJson> animations;
    std::string defaultAnimation;
};

class AnimationSet {
public:
    AnimationSet() = default;
    ~AnimationSet() = default;

    void update();
    void reset();

    // Validators
    bool isAnimation(const std::string& name) const;

    // Getters
    std::string getCurrentAnimationName() const;
    sf::IntRect getCurrentFrame() const;

    // Setters
    void add(const std::string& name, const Animation& animation);
    void play(const std::string& name);
    void setDefaultAnimation(const std::string& name);

private:
    std::string currentAnimationName;
    std::string defaultAnimationName;
    std::map<std::string, Animation> animations;
};


#endif //SFML_MARIO_TEST_ANIMATIONSET_H