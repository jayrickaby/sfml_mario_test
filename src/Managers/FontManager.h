//
// Created by jay on 27/02/2026.
//

#ifndef SFML_MARIO_TEST_FONTMANAGER_H
#define SFML_MARIO_TEST_FONTMANAGER_H
#include <map>
#include <string>

#include "SFML/Graphics.hpp"


class FontManager {
public:
    FontManager() = default;
    ~FontManager() = default;

    static void initialise();

    // Validators
    static bool isFont(const std::string& name);
    static bool isInitialised();

    // Getters
    static sf::Font* getFont(const std::string& name);

private:
    static bool initialised;
    static std::filesystem::path fullPath;
    static std::map<std::string, sf::Font> fonts;
};


#endif //SFML_MARIO_TEST_FONTMANAGER_H