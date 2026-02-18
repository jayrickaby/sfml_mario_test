//
// Created by jay on 18/02/2026.
//

#include "ModelManager.h"

#include <fstream>
#include <spdlog/spdlog.h>

#include "AnimationManager.h"
#include "ManagerUtilities.h"
#include "TextureManager.h"


bool ModelManager::initialised;
std::filesystem::path ModelManager::fullPath;
std::map<std::filesystem::path, Model> ModelManager::models;

void ModelManager::initialise() {
    fullPath = std::filesystem::path("assets/models");
    spdlog::info("Initialising Model Manager...");
    const auto& files = ManagerUtilities::getFilesFromPath(fullPath, {".json"});
    for (const auto& file : files) {
        spdlog::info("Found model: {}", file.string());

        std::ifstream fileContents((fullPath / file).string());
        if (fileContents.peek() == std::ifstream::traits_type::eof()) {
            spdlog::critical("File: \"" + file.string() + "\" has no data!");
            throw std::invalid_argument("Model file has no data!");
        }
        nlohmann::basic_json<> data(nlohmann::json::parse(fileContents));

        models.emplace(file, createModel(parseModelJson(data)));
    }

    initialised = true;
    spdlog::info("Initialised Model Manager!");
}

bool ModelManager::isModel(const std::filesystem::path& path) {
    if (!isInitialised()) {
        spdlog::error("Tried to check if Model \"{}\" exists in uninitialised Model Manager!");
        throw std::runtime_error("Model Manager is uninitialised!");
    }
    return models.contains(path);
}

bool ModelManager::isInitialised() {
    return initialised;
}

Model ModelManager::getModel(const std::filesystem::path& path) {
    if (!isModel(path)) {
        spdlog::error("Tried to get Model \"{}\" but it doesn't exist!", path.string());
        throw std::invalid_argument("Model doesn't exist!");
    }
    return models[path];
}

ModelJson ModelManager::parseModelJson(const nlohmann::basic_json<>& data) {
    if (data.empty()) {
        spdlog::critical("Model has no data!");
        throw std::invalid_argument("Model has no data!");
    }

    ModelJson modelJson;

    // Animation File
    if (data.contains("animationFile")) {
        if (!data["animationFile"].is_string()) {
            spdlog::critical("Invalid animation file path!");
            throw std::invalid_argument("Invalid animation file path!");
        }
        modelJson.animationFile = data["animationFile"].get<std::string>();
    }

    // Texture Files
    if (!data.contains("textureFiles") || data["textureFiles"].empty()) {
        spdlog::critical("No texture paths specified!");
        throw std::invalid_argument("No texture paths specified!");
    }

    for (const auto& file : data["textureFiles"]) {
        std::filesystem::path path = file.get<std::string>();
        if (path.empty()){
            spdlog::critical("Invalid texture file path!");
            throw std::invalid_argument("Invalid texture file path!");
        }

        if (!std::ranges::contains(modelJson.textureFiles, path)) {
            modelJson.textureFiles.emplace_back(path);
        }
    }

    // Default Texture
    if (data.contains("defaultTexture") && data["defaultTexture"].is_string()) {
        modelJson.defaultTexture = data["defaultTexture"].get<std::string>();
    }
    else {
        spdlog::warn("Default texture is invalid. Using first texture in list!");
        modelJson.defaultTexture = modelJson.textureFiles.front();
    }

    return modelJson;
}

Model ModelManager::createModel(const ModelJson& modelJson) {
    Model model;
    if (!modelJson.animationFile.empty()) {
        model.setAnimationSet(AnimationManager::getAnimationSet(modelJson.animationFile));
    }

    for (const auto& file : modelJson.textureFiles) {
        model.addTexture(file, TextureManager::getTexture(file));
    }

    model.setDefaultTexture(modelJson.defaultTexture);
    model.setTexture(model.getDefaultTextureName());

    return model;
}
