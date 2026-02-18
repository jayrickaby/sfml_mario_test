//
// Created by jay on 23/01/2026.
//

#include "AnimationManager.h"

#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>

#include "ManagerUtilities.h"
#include "../Types/AnimationSet.h"
#include "../Types/Animation.h"
#include "../Types/Frame.h"

bool AnimationManager::initialised;
    std::filesystem::path AnimationManager::fullPath;
std::map<std::filesystem::path, AnimationSet> AnimationManager::animationSets;

void AnimationManager::initialise(){
    fullPath = std::filesystem::path("assets/animations");
    spdlog::info("Initialising Animation Manager...");
    const auto& files = ManagerUtilities::getFilesFromPath(fullPath, {".json"});
    for (const auto& file : files) {
        spdlog::info("Found animation set: {}", file.string());

        std::ifstream fileContents((fullPath / file).string());
        if (fileContents.peek() == std::ifstream::traits_type::eof()) {
            spdlog::critical("File: \"" + file.string() + "\" has no data!");
            throw std::invalid_argument("Animation file has no data!");
        }
        nlohmann::basic_json<> data(nlohmann::json::parse(fileContents));

        animationSets.emplace(file.string(), createAnimationSet(parseAnimationSetJson(data)));
    }

    initialised = true;
    spdlog::info("Initialised Animation Manager!");
}

bool AnimationManager::isAnimationSet(const std::filesystem::path& path) {
    if (!isInitialised()) {
        spdlog::error("Tried to check if AnimationSet \"{}\" exists in uninitialised Animation Manager!", path.string());
        throw std::runtime_error("Animation Manager is uninitialised!");
    }
    return animationSets.contains(path);
}
bool AnimationManager::isInitialised() {
    return initialised;
}

AnimationSet AnimationManager::getAnimationSet(const std::filesystem::path& path) {
    if (!isAnimationSet(path)) {
        spdlog::error("Tried to get AnimationSet \"" + path.string() + "\", but it doesn't exist!");
        throw std::invalid_argument("AnimationSet doesn't exist!");
    }
    return animationSets[path];
}

AnimationSetJson AnimationManager::parseAnimationSetJson(const nlohmann::basic_json<>& data) {
    if (data.empty()) {
        spdlog::critical("Animation has no data!");
        throw std::invalid_argument("Animation has no data!");
    }

    AnimationSetJson animationSetJson;

    for (const auto& animation : data["animations"]) {
        AnimationJson animationJson = parseAnimationJson(animation);
        animationSetJson.animations.emplace_back(animationJson);
    }

    if (data.contains("defaultAnimation")
        && data["defaultAnimation"].is_string()
        && !data["defaultAnimation"].empty()
        && std::ranges::count(animationSetJson.animations, data["defaultAnimation"].get<std::string>(), &AnimationJson::name) == 1){
        animationSetJson.defaultAnimation = data["defaultAnimation"].get<std::string>();
    }
    else {
        spdlog::warn("Invalid default animation. Using first in AnimationSet!");
        animationSetJson.defaultAnimation = animationSetJson.animations.front().name;
    }

    return animationSetJson;
}
AnimationJson AnimationManager::parseAnimationJson(const nlohmann::basic_json<>& data) {
    if (data.empty()) {
        spdlog::critical("No animation data!");
        throw std::invalid_argument("Animation has no data!");
    }

    AnimationJson animationJson;

    for (const auto& frame : data["frames"]) {
        FrameJson json = parseFrameJson(frame);
        animationJson.frames.push_back(json);
    }

    if (data.contains("name")
        && data["name"].is_string()
        && !data["name"].empty()) {
        animationJson.name = data["name"].get<std::string>();
    }
    else {
        spdlog::critical("Animation has invalid name!");
        throw std::runtime_error("Animation has invalid name!");
    }

    if (data.contains("speed")
        && data["speed"].is_number()) {
        animationJson.speed = data["speed"].get<float>();
    }
    else {
        spdlog::warn("Invalid animation speed! Using default.");
    }

    return animationJson;
}
FrameJson AnimationManager::parseFrameJson(const nlohmann::basic_json<>& data) {
    if (data.empty()) {
        spdlog::critical("No frame data!");
        throw std::invalid_argument("Frame has no data!");
    }

    FrameJson frameJson;

    if (data.contains("size")
        && data["size"].is_array()) {
        frameJson.size = ManagerUtilities::getVector2iFromJson(data["size"]);
    }
    else {
        spdlog::error("Invalid frame size!");
        throw std::runtime_error("Invalid frame size!");
    }

    if (data.contains("pos")
        && data["pos"].is_array()) {
        frameJson.pos = ManagerUtilities::getVector2iFromJson(data["pos"]);
    }
    else {
        spdlog::error("Invalid frame position!");
        throw std::invalid_argument("Invalid frame position!");
    }
    if (data.contains("duration")
        && data["duration"].is_number()) {
        frameJson.duration = data["duration"].get<float>();
    }
    else {
        spdlog::warn("Invalid frame duration! Using default.");
    }

    return frameJson;
}

AnimationSet AnimationManager::createAnimationSet(const AnimationSetJson& animationSetJson) {
    AnimationSet animationSet;

    for (const auto& animation : animationSetJson.animations) {
        animationSet.add(animation.name, createAnimation(animation));
    }

    animationSet.setDefaultAnimation(animationSetJson.defaultAnimation);

    return animationSet;
}
Animation AnimationManager::createAnimation(const AnimationJson& animationJson) {
    Animation animation;

    for (const auto& frame : animationJson.frames) {
        animation.frames.emplace_back(createFrame(frame));
    }

    animation.defaultSpeed = animationJson.speed;
    animation.speed = animationJson.speed;

    return animation;
}
Frame AnimationManager::createFrame(const FrameJson& frameJson) {
    Frame frame;

    frame.rect = sf::IntRect(frameJson.pos, frameJson.size);
    frame.duration = frameJson.duration;

    return frame;
}
