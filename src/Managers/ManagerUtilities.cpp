//
// Created by jay on 08/02/2026.
//

#include "ManagerUtilities.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "AnimationManager.h"

std::vector<std::string> ManagerUtilities::findFiles(const std::string& path, const std::vector<std::string>& extensions){
    std::cout << "Looking in: \"" << path << "\"" << std::endl;
    if (!std::filesystem::is_directory(path)){
        throw std::runtime_error("Could not find directory: \"" + path + "\"");
    }
    const std::filesystem::path directory{path};

    std::vector<std::string> files;
    for (auto const& dirEntry : std::filesystem::recursive_directory_iterator{directory}){

        std::string relativeFilePath = std::filesystem::relative(dirEntry, path).string();
        // Only accept extensions in vector
        bool accepted = false;
        for (std::string extension : extensions){
            if (extension == dirEntry.path().extension().string()){
                files.emplace_back(relativeFilePath);
                std::cout << "Found file: \"" << (relativeFilePath) << "\"" << std::endl;
                accepted = true;
                break;
            }
            if (dirEntry.is_directory()){
                accepted = true;
            }
        }
        if (!accepted){
            std::cout << "Rejected file: " << (relativeFilePath) << std::endl;
        }
    }
    return files;
}

std::string ManagerUtilities::getStringFromJson(const nlohmann::basic_json<>& data) {
    if (data.size() != 1) {
        std::string msg = "Data does not contain 1 element!\nSize: " + std::to_string(data.size()) + "\n";
        throw std::runtime_error(msg);
    }
    std::string string = data.get<std::string>();
    return string;
}

sf::Vector2f ManagerUtilities::getVector2fFromJson(const nlohmann::json& data) {
    if (data.size() != 2) {
        std::string msg = "Data does not contain 2 elements!\nSize: " + std::to_string(data.size()) + "\n";
        throw std::runtime_error(msg);
    }

    sf::Vector2f vector;
    vector.x = data[0].get<float>();
    vector.y = data[1].get<float>();
    return vector;
}

sf::Vector2i ManagerUtilities::getVector2iFromJson(const nlohmann::json& data) {
    if (data.size() != 2) {
        std::string msg = "Data does not contain 2 elements!\nSize: " + std::to_string(data.size()) + "\n";
        throw std::runtime_error(msg);
    }

    sf::Vector2i vector;
    vector.x = data[0].get<float>();
    vector.y = data[1].get<float>();
    return vector;
}
