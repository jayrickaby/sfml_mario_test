//
// Created by jay on 09/01/2026.
//

#ifndef SFML_MARIO_TEST_TEXTUREMANAGER_H
#define SFML_MARIO_TEST_TEXTUREMANAGER_H
#include <filesystem>
#include <vector>
#include <map>

#include "SFML/Graphics/Texture.hpp"

struct Texture;

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() = default;

    static void initialise();

    // Validators
    static bool isTexture(const std::filesystem::path& path);
    static bool isInitialised();

    // Getters
    static sf::Texture* getAtlas();
    static Texture* getTexture(const std::filesystem::path& path);

private:
    static sf::Texture createAtlas(const std::vector<std::filesystem::path>& files);

    static bool initialised;
    static std::filesystem::path fullPath;

    static std::map<std::filesystem::path, Texture> textures;
    static sf::Texture atlas;
};


#endif //SFML_MARIO_TEST_TEXTUREMANAGER_H