//
// Created by jay on 08/02/2026.
//

#ifndef SFML_MARIO_TEST_MANAGERUTILITIES_H
#define SFML_MARIO_TEST_MANAGERUTILITIES_H
#include <filesystem>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "SFML/System/Vector2.hpp"


class ManagerUtilities {
public:
    ManagerUtilities() = default;
    ~ManagerUtilities() = default;

    static std::vector<std::filesystem::path> getFilesFromPath(const std::filesystem::path& path);
    static std::vector<std::filesystem::path> getFilesFromPath(const std::filesystem::path& path, const std::vector<std::string>& extensions);

    static bool isFileEmpty(std::ifstream& data);

    static sf::Vector2i getVector2iFromJson(const nlohmann::basic_json<>& data);
};


#endif //SFML_MARIO_TEST_MANAGERUTILITIES_H