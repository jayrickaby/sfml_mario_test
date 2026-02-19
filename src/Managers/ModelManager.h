//
// Created by jay on 18/02/2026.
//

#ifndef SFML_MARIO_TEST_MODELMANAGER_H
#define SFML_MARIO_TEST_MODELMANAGER_H
#include <filesystem>
#include <nlohmann/json.hpp>

#include "../Types/Model.h"

class ModelManager {
public:
    ModelManager() = default;

    static void initialise();

    // Validators
    static bool isModel(const std::filesystem::path& path);
    static bool isInitialised();

    // Getters
    static Model getModel(const std::filesystem::path& path);

private:
    static ModelJson parseModelJson(const nlohmann::basic_json<>& data);

    static Model createModel(const ModelJson& modelJson);

    static bool initialised;
    static std::filesystem::path fullPath;
    static std::map<std::filesystem::path, Model> models;

};


#endif //SFML_MARIO_TEST_MODELMANAGER_H