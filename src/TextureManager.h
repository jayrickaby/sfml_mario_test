//
// Created by jay on 09/01/2026.
//

#ifndef SFML_MARIO_TEST_TEXTUREMANAGER_H
#define SFML_MARIO_TEST_TEXTUREMANAGER_H

#include <map>
#include <SFML/Graphics.hpp>

// @TODO implement atlassing
struct TextureAtlas{
    sf::Texture atlasTexture;
    std::map<std::string, sf::IntRect> textures;
};

class TextureManager{
public:
    TextureManager() = default;
    ~TextureManager() = default;

    static void initialiseTextures();
    static bool isTexture(const std::string& path);
    static bool isInitialised();
    static sf::Texture& loadTexture(const std::string& path);

private:
    static std::map<std::string,sf::Texture> textures;
};


#endif //SFML_MARIO_TEST_TEXTUREMANAGER_H