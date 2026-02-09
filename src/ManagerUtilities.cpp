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
