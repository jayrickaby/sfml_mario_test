//
// Created by jay on 23/01/2026.
//

#ifndef SFML_MARIO_TEST_ANIMATIONMANAGER_H
#define SFML_MARIO_TEST_ANIMATIONMANAGER_H

#include <nlohmann/json.hpp>
#include "Animation.h"

class Animation;

class AnimationManager{
public:
    AnimationManager() = default;
    ~AnimationManager() = default;

    static void initialiseAnimations();
    static std::map<std::string, Animation>* loadAnimation(const std::string& name);

private:
    // e.g.             Player                 Idle     [data]
    static std::map<std::string, std::map<std::string, Animation>> animations;

    static Frame parseFrame(const nlohmann::basic_json<>& frameData);
    static std::map<std::string, Animation> parseAnimations(const std::string& path);
    static Animation parseAnimation(const nlohmann::basic_json<>& name);
};


#endif //SFML_MARIO_TEST_ANIMATIONMANAGER_H