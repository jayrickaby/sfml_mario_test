//
// Created by jay on 20/02/2026.
//

#ifndef SFML_MARIO_TEST_LEVELMANAGER_H
#define SFML_MARIO_TEST_LEVELMANAGER_H
#include <string>

#include "nlohmann/json.hpp"


struct LevelJson;
struct Level;

class LevelManager {
public:
    LevelManager() = default;
    ~LevelManager() = default;

    static void initialise();

    // Validators
    static bool isLevel(const std::filesystem::path& path);
    static bool isInitialised();

    // Getters
    static Level getLevel(const std::filesystem::path& path);

private:
    static LevelJson parseLevelJson(const nlohmann::basic_json<>& data);

    static Level createLevel(const LevelJson& levelJson);

    static bool initialised;
    static std::filesystem::path fullPath;
    static std::vector<std::filesystem::path> levels;
};


#endif //SFML_MARIO_TEST_LEVELMANAGER_H