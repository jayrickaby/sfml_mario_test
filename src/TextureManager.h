//
// Created by jay on 09/01/2026.
//

#ifndef SFML_MARIO_TEST_TEXTUREMANAGER_H
#define SFML_MARIO_TEST_TEXTUREMANAGER_H

#include <map>
#include <SFML/Graphics.hpp>

class TextureManager{
public:
    TextureManager();
    ~TextureManager();

    static void initialiseTextures(const std::string& path);
    static sf::Texture& loadTexture(const std::string& path);

private:
    static std::map<std::string,sf::Texture> textures;
};


#endif //SFML_MARIO_TEST_TEXTUREMANAGER_H