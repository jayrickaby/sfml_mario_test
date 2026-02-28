//
// Created by jay on 27/02/2026.
//

#include "FontManager.h"

#include "ManagerUtilities.h"
#include "spdlog/spdlog.h"

bool FontManager::initialised = false;
std::filesystem::path FontManager::fullPath;

std::map<std::string, sf::Font> FontManager::fonts;

void FontManager::initialise() {
    fullPath = std::filesystem::path("assets/fonts");
    spdlog::info("Initialising Font Manager...");
    const auto& files = ManagerUtilities::getFilesFromPath(fullPath, {".ttf"});
    for (const auto& file : files) {
        spdlog::info("Found font: {}", file.string());
        fonts.emplace(file.stem(), sf::Font(fullPath / file));
    }
    initialised = true;
    spdlog::info("Initialised Font Manager!");
}

bool FontManager::isFont(const std::string& name) {
    if (!isInitialised()) {
        spdlog::error("Tried to check if Font \"{}\" exists in uninitialised Font Manager!", name);
        throw std::runtime_error("Font Manager is uninitialised!");
    }
    return fonts.contains(name);
}

bool FontManager::isInitialised() {
    return initialised;
}

sf::Font* FontManager::getFont(const std::string& name) {
    if (!isFont(name)) {
        spdlog::error("Font \"{}\" doesn't exit!", name);
        throw std::invalid_argument("Font Manager is uninitialised!");
    }
    return &fonts[name];
}
