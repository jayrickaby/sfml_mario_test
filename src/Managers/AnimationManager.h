//
// Created by jay on 23/01/2026.
//

#ifndef SFML_MARIO_TEST_ANIMATIONMANAGER_H
#define SFML_MARIO_TEST_ANIMATIONMANAGER_H
#include <nlohmann/json.hpp>

#include "../Types/AnimationSet.h"
#include "../Types/Animation.h"
#include "../Types/Frame.h"

class AnimationManager {
public:
    AnimationManager() = default;
    ~AnimationManager() = default;

    static void initialise();

    // Validators
    static bool isAnimationSet(const std::filesystem::path& path);
    static bool isInitialised();

    // Getters
    static AnimationSet getAnimationSet(const std::filesystem::path& path);

private:
    static AnimationSetJson parseAnimationSetJson(const nlohmann::basic_json<>& data);
    static AnimationJson parseAnimationJson(const nlohmann::basic_json<>& data);
    static FrameJson parseFrameJson(const nlohmann::basic_json<>& data);

    static AnimationSet createAnimationSet(const AnimationSetJson& animationSetJson);
    static Animation createAnimation(const AnimationJson& animationJson);
    static Frame createFrame(const FrameJson& frameJson);

    static bool initialised;
    static std::filesystem::path fullPath;
    static std::map<std::filesystem::path, AnimationSet> animationSets;
};


#endif //SFML_MARIO_TEST_ANIMATIONMANAGER_H