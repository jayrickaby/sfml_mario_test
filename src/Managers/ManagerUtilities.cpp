//
// Created by jay on 08/02/2026.
//

#include "ManagerUtilities.h"

#include <filesystem>
#include <fstream>

#include "SFML/System/Vector2.hpp"

std::vector<std::filesystem::path> ManagerUtilities::getFilesFromPath(const std::filesystem::path& path,
                                                                      const std::vector<std::string>& extensions) {
    if (!std::filesystem::is_directory(path)){
        throw std::runtime_error("Could not find directory: \"" + path.string() + "\"");
    }
    std::vector<std::filesystem::path> files;

    for (const auto& entry : std::filesystem::recursive_directory_iterator{path}) {
        const std::filesystem::path relativePath{std::filesystem::relative(entry, path)};
        if (!extensions.empty()){
            for (const auto& extension : extensions) {
                if (extension == entry.path().extension().string()) {
                    files.emplace_back(relativePath);
                    break;
                }
                if (entry.is_directory()) {
                    break;
                }
            }
        }
        else {
            files.emplace_back(relativePath);
        }
    }
    return files;
}
std::vector<std::filesystem::path> ManagerUtilities::getFilesFromPath(const std::filesystem::path& path) {
    return getFilesFromPath(path, {});
}

bool ManagerUtilities::isFileEmpty(std::ifstream& data) {
    if (data.peek() == std::ifstream::traits_type::eof()) {
        return true;
    }
    return false;
}

sf::Vector2i ManagerUtilities::getVector2iFromJson(const nlohmann::basic_json<>& data) {
    sf::Vector2i vec;

    if (!data[0].is_number_integer()) {
        vec.x = 0;
    }
    else {
        vec.x = data[0].get<int>();
    }

    if (!data[1].is_number_integer()) {
        vec.y = 0;
    }
    else {
        vec.y = data[1].get<int>();
    }

    return vec;
}
